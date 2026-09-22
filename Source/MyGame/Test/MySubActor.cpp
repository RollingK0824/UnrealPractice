// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/MySubActor.h"

// Sets default values
AMySubActor::AMySubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySubActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMySubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

