// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/MyYoutuber.h"

// Sets default values
AMyYoutuber::AMyYoutuber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyYoutuber::BeginPlay()
{
	Super::BeginPlay();

	// 2초뒤에 방송 켜기 CreateWeakLambda : this 가 무효화되면, 람다도 무효화.
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([this]()
	{
		// 2초뒤에 실행할 함수. 즉, 구독자에게 알람 보낸다.
		//OnAirStart.ExecuteIfBound();

		OnAirStart.Broadcast(); // 멀티캐스트 델리게이트는 Broadcast() 호출

	}), 2.0f, false);
}

// Called every frame
void AMyYoutuber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

