// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class UNREALPRACTICE_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ATPSPlayer();

	void Turn(const struct FInputActionValue& inputValue);
	void LookUp(const struct FInputActionValue& inputValue);
	void Move(const struct FInputActionValue& inputValue);
	void InputJump(const struct FInputActionValue& inputValue);
	void InputFire(const struct FInputActionValue& inputValue);
	void ChangeToAssaultRifle(const struct FInputActionValue& inputValue);
	void ChangeToSniperRifle(const struct FInputActionValue& inputValue);
	void PlayerMove();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* GunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* SniperGunComp;

	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> BulletFactory;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TPSCamComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_TPS;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_AssaultRifle;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SniperRifle;

	bool bUsingAssaultRifle = true;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float WalkSpeed = 600;

	FVector direction;

	UPROPERTY(EditAnywhere, Category = BulletEffect)
	TObjectPtr<class UNiagaraSystem> BulletEffectFactory;
};
