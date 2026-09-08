// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerMove.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
void UPlayerMove::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	Me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	Me->AddControllerPitchInput(value);
}

void UPlayerMove::SetupInputBinding(UEnhancedInputComponent* playerInput)
{
	playerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &UPlayerMove::Turn);
	playerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &UPlayerMove::LookUp);
}