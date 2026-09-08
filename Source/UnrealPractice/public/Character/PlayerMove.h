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
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	void Turn(const struct FInputActionValue& inputValue);
	void LookUp(const struct FInputActionValue& inputValue);

public:
	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput)override;
};