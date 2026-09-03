#include "Character/PlayerAnim.h"
#include "Character/TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

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
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(AttackAnimMontage);
}
