#include "Character/PlayerAnim.h"
#include "Character/TPSPlayer.h"
#include "Character/PlayerFire.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Weapon/WeaponBase.h"
void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ATPSPlayer>(TryGetPawnOwner());
	if (Character != nullptr)
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

		AO_Yaw = Character->AO_Yaw;
		AO_Pitch = Character->AO_Pitch;
	}
	if (!Character)return;
	if (!Character->PlayerFire)return;
	if (AWeaponBase* Weapon = Character->PlayerFire->CurrentWeapon)
	{
		// 무기의 LeftHandSocket 위치를 월드 좌표로 가져온다.
		FTransform LeftHandWorldTransform = Weapon->WeaponMesh->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);

		FVector OutPosition;
		FRotator OutRotator;
		Character->GetMesh()->TransformToBoneSpace(
			FName("hand_r"),
			LeftHandWorldTransform.GetLocation(),
			FRotator::ZeroRotator,
			OutPosition,
			OutRotator);

		// hand_r 공간으로 변환한 좌표를 animation 좌표로 설정한다.
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotator));
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	bIsShot = true;

	GetWorld()->GetTimerManager().SetTimer(
		StateResetTimerHandle,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				bIsShot = false;
			}),
		1.5f,
		false
	);
	Montage_Play(AttackAnimMontage);
}
