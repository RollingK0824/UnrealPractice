#include "Character/PlayerAnim.h"
#include "Character/TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Kismet/KismetSystemLibrary.h"
#include "Component/PlayerFire.h"
#include "Weapon/MyWeapon.h"

void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 초기화 될때 캐싱해두기
	Character = Cast<ATPSPlayer>(GetOwningActor());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character && MovementComponent)
	{
		// 속도 계산
		Velocity = Character->GetVelocity();
		GroundSpeed = Velocity.Length();
		bShouldMove = (MovementComponent->GetCurrentAcceleration().Size() > 0) && (GroundSpeed >= 0.01f);
		bIsFalling = MovementComponent->IsFalling();

		// 방향 계산
		Direction = CalculateDirection(Velocity, Character->GetActorRotation());

		// 에임 오프셋 계산
		AO_Yaw = Character->AO_Yaw;
		AO_Pitch = Character->AO_Pitch;
	}

	// 왼손이 있어야할 IK 위치 계산
	if (Character && Character->playerFire && Character->playerFire->CurrentWeapon && Character->playerFire->CurrentWeapon->WeaponMesh)
	{
		FTransform LeftHandWorldTransform = Character->playerFire->CurrentWeapon->WeaponMesh->GetSocketTransform(TEXT("LeftHandSocket"), ERelativeTransformSpace::RTS_World);

		FVector OutPosition;
		FRotator OutRotator;
		Character->GetMesh()->TransformToBoneSpace(FName("hand_r"),
			LeftHandWorldTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotator);

		// hand_r 공간으로 변환한 좌표를 animation 좌표로 설정한다.
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotator));
	}

	// 마지막으로 총을 발사한 시간이 충분히 지났는지 확인
	double TimeSinceLastFire = GetWorld()->GetTimeSeconds() - LastFireTime;
	bUseFireIdle = (TimeSinceLastFire < FireIdleTime);
}

void UPlayerAnim::PlayAttackAnim()
{
	LastFireTime = GetWorld()->GetTimeSeconds();
	Montage_Play(attackAnimMontage);
}
