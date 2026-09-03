#include "Enemy/EnemyFSM.h"
#include "Character/TPSPlayer.h"
#include "Enemy/Enemy.h"
#include <Kismet/GamePlayStatics.h>
#include "../UnrealPractice.h"
#include <Components/CapsuleComponent.h>
UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	Target = Cast<ATPSPlayer>(actor);
	Me = Cast<AEnemy>(GetOwner());
}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(MState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);

	switch (MState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleDelayTime)
	{
		MState = EEnemyState::Move;
		CurrentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = Target->GetActorLocation();
	FVector dir = destination - Me->GetActorLocation();
	Me->AddMovementInput(dir.GetSafeNormal());

	if (dir.Size() < AttackRange)
	{
		MState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > AttackDelayTime)
	{
		PRINT_LOG(TEXT("Attack!!!!!"));
		CurrentTime = 0;
	}

	float distance = FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation());

	if (distance > AttackRange)
	{
		MState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > DamageDelayTime)
	{
		MState = EEnemyState::Idle;
		CurrentTime = 0;
	}
}

void UEnemyFSM::DieState()
{
	FVector P0 = Me->GetActorLocation();
	FVector vt = FVector::DownVector * DieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	Me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		Me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	Hp--;
	if (Hp > 0)
	{
		MState = EEnemyState::Damage;
	}
	else
	{
		MState = EEnemyState::Die;
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
