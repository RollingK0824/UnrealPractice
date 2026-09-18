#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AssaultRifle       UMETA(DisplayName = "Assault Rifle"),
	RocketLauncher      UMETA(DisplayName = "Rocket Launcher"),
	SniperRifle      UMETA(DisplayName = "Sniper Rifle"),
	Pistol      UMETA(DisplayName = "Pistol"),
	Shotgun     UMETA(DisplayName = "Shotgun"),
	Knife       UMETA(DisplayName = "Knife"),
	NONE       UMETA(DisplayName = "None"),
};

UCLASS()
class UNREALPRACTICE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::NONE;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon");
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon");
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

public:
	UPROPERTY(EditDefaultsOnly, Category = "BulletFactory")
	TSubclassOf<class ABullet> BulletFactory;

public:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> EffectFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> BeamParticles;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FString FirePosSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	FVector EffectPos;


	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	TObjectPtr<class UMaterialInterface> DecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	FVector DecalSize = FVector(10.0f, 10.0f, 10.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	float DecalLifetime = 10.0f;


public:
	UPROPERTY(EditDefaultsOnly, Category = "CameraMotion")
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* FireSound;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<class UAnimInstance> WeaponAnimLayerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> AttackMontage;

public:
	virtual void PrimaryAction();
	virtual void SecondaryAction();
	virtual void Fire();
};
