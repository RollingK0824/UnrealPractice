#include "System/EnemyManager.h"
#include "Enemy/Enemy.h"
#include <EngineUtils.h>
#include<Kismet/GameplayStatics.h>

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	float createTime = FMath::RandRange(MinTime, MaxTime);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);

	FindSpawnPoints();
}

void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemy()
{
	int index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	GetWorld()->SpawnActor<AEnemy>(EnemyFactory, SpawnPoints[index]->GetActorLocation(), FRotator(0));

	float createTime = FMath::RandRange(MinTime, MaxTime);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
}

void AEnemyManager::FindSpawnPoints()
{
	/*for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* spawn = *It;
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}*/
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

