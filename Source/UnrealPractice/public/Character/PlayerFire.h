// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent),meta = (BlueprintSpawnableComponent))
class UNREALPRACTICE_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY() 

public:
	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput)override;

public:
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> BulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	void InputFire(const struct FInputActionValue& inputValue);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_NextWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PrevWeapon;

	bool bUsingSniperRifle = true;

	void Input_ChangeToNextWeapon(const struct FInputActionValue& inputValue);
	void Input_ChangeToPrevWeapon(const struct FInputActionValue& inputValue);
	void EquipWeapon();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SniperZoom;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY()
	class UUserWidget* SniperUI;

	bool bSniperAim = false;

	void SniperAim(const struct FInputActionValue& inputValue);

public:
	UPROPERTY(EditAnywhere, Category = BulletEffect)
	TObjectPtr<class UNiagaraSystem> BulletEffectFactory;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* BulletSound;

public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TPSCamComp = nullptr;

public:
	UPROPERTY(EditDefaultsOnly, Category = "MyGame")
	TObjectPtr<class UMaterialInterface> BulletDecalMaterial;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UNiagaraSystem> BeamParticles;
	
	UPROPERTY(EditDefaultsOnly, Category = "MyGame")
	FVector DecalSize = FVector(10.0f, 10.0f, 10.0f);
	UPROPERTY(EditDefaultsOnly, Category = "MyGame")
	float DecalLifetime = 10.0f;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AMyWeapon>> StartingWeapons;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class AMyWeapon>> WeaponList;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AMyWeapon> CurrentWeapon = nullptr;

	INT8 CurrentWeaponIdx = 0;

};