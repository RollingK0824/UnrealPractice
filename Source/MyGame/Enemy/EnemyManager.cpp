#include "Enemy/EnemyManager.h"
#include "Enemy/Enemy.h"
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>
#include "SpawnPoint.h"

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == false)
		return;
	
	// 1. 랜덤 생성 시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	// 2. Timer Manager한테 알람 등록
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this,
		&AEnemyManager::CreateEnemy, createTime);

	// 스폰 위치 동적 할당
	FindSpawnPoints();
}

void AEnemyManager::CreateEnemy()
{
	if (enemyFactory == nullptr)
		return;

	TArray<AActor*> EnemyList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), enemyFactory, EnemyList);

	// 최대치만큼 스폰된 경우 무시
	if (EnemyList.Num() >= maxCount)
		return;


	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// 랜덤 위치 구하기
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);
	// 적 생성 및 배치하기
	GetWorld()->SpawnActor<AActor>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0), SpawnParams);

	// 다시 랜덤 시간에 CreateEnemy 함수가 호출되도록 타이머 설정
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this,
		&AEnemyManager::CreateEnemy, createTime);
}

// 모든 액터 대신 ASpawnPoint 타입의 액터만 검색하도록 변경
void AEnemyManager::FindSpawnPoints()
{
	// 검색으로 찾은 결과를 저장할 배열
	TArray<AActor*> allActors;
	// 원하는 타입의 액터 모두 찾아오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), allActors);
	// 찾은 결과가 있을 경우 반복적으로
	for (auto spawn : allActors)
	{
		//if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			// 스폰 목록에 추가
			spawnPoints.Add(spawn);
		}
	}
}

// 책 원본 코드
//void AEnemyManager::FindSpawnPoints()
//{
//	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
//	{
//		AActor* spawn = *It;
//		// 찾은 액터의 이름에 해당 문자열을 포함하고 있다면
//		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
//		{
//			// 스폰 목록에 추가
//			spawnPoints.Add(spawn);
//		}
//	}
//}

