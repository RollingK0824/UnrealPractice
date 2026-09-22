// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	// 매 프레임 갱신되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerAnim)
	TObjectPtr<class ATPSPlayer> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerAnim)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float Direction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float GroundSpeed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool bIsFalling = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool bShouldMove = false;

	// 재생할 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category=PlayerAnim)
	class UAnimMontage* attackAnimMontage;

	// 총쏘는 상체 애니메이션 여부
	UPROPERTY(BlueprintReadWrite, Category = PlayerAnim)
	bool bUseFireIdle;

	UPROPERTY(EditAnywhere, Category = PlayerAnim)
	float FireIdleTime = 1.5f;	// FireIdle 유지할 시간
	double LastFireTime = -1;


	// 공격 애니메이션 재생 함수
	void PlayAttackAnim();
	
	// 왼손 IK 위치
	UPROPERTY(BlueprintReadWrite, Category = PlayerAnim)
	FTransform LeftHandTransform;

	// 에임 오프셋값
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float AO_Yaw = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float AO_Pitch = 0;
};
