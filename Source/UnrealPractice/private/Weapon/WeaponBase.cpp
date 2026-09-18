#include "Weapon/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/Bullet.h"
#include <Kismet/GameplayStatics.h>
#include "Components/CapsuleComponent.h"
#include "Character/TPSPlayer.h"
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	CollisionComp->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionComp->SetSphereRadius(64);

	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(CollisionComp);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::PrimaryAction()
{
	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);

	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);
	}

	Fire();
}

void AWeaponBase::SecondaryAction()
{

}

void AWeaponBase::Fire()
{
	auto me = GetOwner()->GetComponentByClass<ATPSPlayer>();
	FTransform firePosition = WeaponMesh->GetSocketTransform(TEXT("FirePosition"));
	ABullet* spawnBullet = GetWorld()->SpawnActorDeferred<ABullet>(BulletFactory, firePosition);
	spawnBullet->collisionComp->IgnoreActorWhenMoving(me, true);
	me->GetCapsuleComponent()->IgnoreActorWhenMoving(spawnBullet, true);
	spawnBullet->FinishSpawning(firePosition);

}
