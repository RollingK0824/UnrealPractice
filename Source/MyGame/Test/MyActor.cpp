// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/MyActor.h"
#include "Test/MySubActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	SubActor = GetWorld()->SpawnActor<AMySubActor>();
	SubActor2 = GetWorld()->SpawnActor<AMySubActor>();

	GEngine->ForceGarbageCollection();
	SubActor->Destroy();
	SubActor2->Destroy();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SubActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid SubActor"));
	}

	if (SubActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Null SubActor "));
	}

	if (SubActor2.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid SubActor"));
	}

	if (SubActor2 != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Null SubActor "));
	}
}

