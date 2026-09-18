// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (PlayerComponent), meta = (BlueprintSpawnableComponent))
class UNREALPRACTICE_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput)override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PrimaryAction;
	void InputPrimaryAction(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SecondaryAction;
	void InputSecondaryAction(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_NextWeapon;
	void Input_ChangeToNextWeapon(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PrevWeapon;
	void Input_ChangeToPrevWeapon(const struct FInputActionValue& inputValue);

	void EquipWeapon();
//
//public:
//	UPROPERTY(VisibleAnywhere, Category = "Camera")
//	class USpringArmComponent* SpringArmComp = nullptr;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
//	class UCameraComponent* TPSCamComp = nullptr;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AWeaponBase>> StartingWeapons;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class AWeaponBase>> WeaponList;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AWeaponBase> CurrentWeapon = nullptr;

	INT8 CurrentWeaponIdx = 0;

};