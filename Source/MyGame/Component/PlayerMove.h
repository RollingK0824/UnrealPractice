// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:

	// 마우스 입력 처리
	void Input_Look(const struct FInputActionValue& inputValue);
	// 이동
	void Input_Move(const struct FInputActionValue& inputValue);

	virtual void SetupInputBinding(class UEnhancedInputComponent* PlayerInput) override;
public:

	// 이동 속도
	UPROPERTY(EditAnywhere, Category = "IHGame|Value")
	float walkSpeed = 200;
	// 달리기 속도
	UPROPERTY(EditAnywhere, Category = "IHGame|Value")
	float runSpeed = 600;

	// 달리기
	void Input_Run(const struct FInputActionValue& inputValue);

	// 점프 입력 이벤트 처리 함수
	void Input_Jump(const struct FInputActionValue& inputValue);
};
