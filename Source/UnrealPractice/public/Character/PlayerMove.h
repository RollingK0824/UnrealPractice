#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 *
 */
UCLASS()
class UNREALPRACTICE_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	void Turn(const struct FInputActionValue& inputValue);
	void LookUp(const struct FInputActionValue& inputValue);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	FVector direction;

	void Move(const struct FInputActionValue& inputValue);

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float WalkSpeed = 200;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float RunSpeed = 600;

	void PlayerMove();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerRun;

	void InputRun();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;

	void InputJump(const struct FInputActionValue& inputValue);

public:
	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput)override;
};