// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "SniperRifle.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPRACTICE_API ASniperRifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY()
	TObjectPtr<class UUserWidget> SniperUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> TPSCamComp = nullptr;

public:
	virtual void SecondaryAction() override;
	virtual void Fire() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
	bool bSniperAim = false;
};
