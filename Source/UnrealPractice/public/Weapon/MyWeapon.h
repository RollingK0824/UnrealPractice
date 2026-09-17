// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle       UMETA(DisplayName = "Rifle"),
	Rocket      UMETA(DisplayName = "Rocket Launcher"),
	Sniper      UMETA(DisplayName = "Sniper Rifle"),
};

UCLASS()
class UNREALPRACTICE_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "MyGame");
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "MyGame");
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Rifle;

};
