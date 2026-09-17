#include "Weapon/Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	if (collisionComp)
	{
		collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
		collisionComp->SetSphereRadius(13);
		RootComponent = collisionComp;
	}

	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(5));
	bodyMeshComp->SetRelativeRotation(FRotator(0, -90, 0));

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	movementComp->InitialSpeed = 2000;
	movementComp->MaxSpeed = 2000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;
	movementComp->bRotationFollowsVelocity = true;

	InitialLifeSpan = 2.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);

	TWeakObjectPtr<ABullet> WeakBullet = this;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
		FTimerDelegate::CreateLambda([this]()->void
			{
				Destroy();
			}), 2.0f, false);

}

void ABullet::EndPlay(const EEndPlayReason::Type EndPlay)
{
	Super::EndPlay(EndPlay);

	GetWorld()->GetTimerManager().ClearTimer(deathTimer);
}


void ABullet::Die()
{
	Destroy();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

#if WITH_EDITOR

void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		movementComp->InitialSpeed = speed;
		movementComp->MaxSpeed = speed;
	}
}
#endif
