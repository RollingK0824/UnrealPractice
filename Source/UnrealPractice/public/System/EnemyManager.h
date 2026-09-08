#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class UNREALPRACTICE_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, CateGory=SpawnSettings)
	float MinTime = 1;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float MaxTime = 5;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TArray<class AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TSubclassOf<class AEnemy> EnemyFactory;

	FTimerHandle SpawnTimerHandle;

	void CreateEnemy();

	void FindSpawnPoints();
};