// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class MYGAME_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire();
	void SniperAim();

public:
	// 충돌체크를 위한 캡슐 컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = "MyGame");
	TObjectPtr<class USphereComponent> CollisionComp;

	// 비주얼적인 총기 메시 스켈레탈 컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = "MyGame");
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	// 총알 공장
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	TSubclassOf<class ABullet> bulletFactory;

	// 주무기 여부
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	bool bIsMainWeapon = true;

	// 유탄총 여부
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	bool bUsingGrenadeGun = true;

	// 스나이퍼 조준 중인지 여부
	bool bSniperAim = false;

	// 스나이퍼 UI 위젯 공장
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	TSubclassOf<class UUserWidget> sniperUIFactory;

	// 스나이퍼 UI 위젯 인스턴스
	UPROPERTY()
	class UUserWidget* sniperUI;

	// 카메라 셰이크 블루프린트를 저장할 변수
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// 히트시 터지는 이펙트
	UPROPERTY(EditAnywhere, Category = "IHGame|Weapon")
	TObjectPtr<class UNiagaraSystem> bulletEffectFactory;

	// 총알 발사 사운드
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	class USoundBase* bulletSound;

	// 탄흔 머티리얼
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	TObjectPtr<class UMaterialInterface> BulletDecalMaterial;
	// 탄흔 사이즈
	UPROPERTY(EditAnywhere, Category = "IHGame|Weapon")
	FVector DecalSize = FVector(10.0f, 10.0f, 10.0f);
	// 탄흔 유지 시간
	UPROPERTY(EditAnywhere, Category = "IHGame|Weapon")
	float DecalLifetime = 10.f;

	// 빔 트레일 (궤적 흔적?)
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	TObjectPtr<class UNiagaraSystem> BeamParticles;

	// UI Icon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IHGame|Weapon")
	TObjectPtr<class UTexture2D> TextureIcon;

	UPROPERTY(EditAnywhere, Category = "IHGame|Weapon")
	TObjectPtr<UAnimationAsset> FireWeaponAnimation;
};
