// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS()
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
	class UInputAction* IA_AssaultRifle;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SniperRifle;

	bool bUsingAssaultRifle = true;

	void ChangeToAssaultRifle(const struct FInputActionValue& inputValue);
	void ChangeToSniperRifle(const struct FInputActionValue& inputValue);

public:
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
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* GunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* SniperGunComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TPSCamComp = nullptr;
};