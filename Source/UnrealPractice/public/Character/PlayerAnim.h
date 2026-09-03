// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPRACTICE_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackAnim();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category=PlayerAnim)
	class UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float GroundSpeed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float Direction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool bShouldMove = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool bIsFalling = false;
	
};
