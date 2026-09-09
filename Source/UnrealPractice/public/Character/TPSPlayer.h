// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*);

UCLASS()
class UNREALPRACTICE_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ATPSPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FInputBindingDelegate OnInputBindingDelegate;

public:
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* GunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* SniperGunComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TPSCamComp = nullptr;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_TPS;

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* PlayerMove;

	/*UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* PlayerFire;*/
};
