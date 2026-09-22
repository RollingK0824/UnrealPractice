// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/MySubscribe.h"
#include "Test/MyYoutuber.h"
#include "MyGame.h"

// Sets default values
AMySubscribe::AMySubscribe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 가끔가다 생성자에서 바인딩하는 경우가 있다.
	// 생성자에서 바인딩하는 행위는 권장하지 않는다.
	// 블루프린트와 호환되는 AddDynamic() 이거는 무조건 생성자에서 하지 마세요.
	//Youtuber->OnAirStart.AddUObject(this, &AMySubscribe::Watch);
}

// Called when the game starts or when spawned
void AMySubscribe::BeginPlay()
{
	Super::BeginPlay();

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyYoutuber::StaticClass(), Youtubers);
	if (Youtuber)
	{
		// 본인 함수를 바인딩 하는 방법
		//Youtuber->OnAirStart.AddUObject(this, &AMySubscribe::Watch);

		// UFUNCTION 으로 바인딩 하는 방법 : 반드시 UFUNCTION() 붙여야 합니다.
		//Youtuber->OnAirStart.AddUFunction(this, TEXT("Watch"));

		// 람다 델리게이트 바인딩
		//Youtuber->OnAirStart.AddLambda([this]()
		//	{
		//		Watch();
		//	});

		// 블루프린트 버전을 함수 포인터로 연결할때는 AddDynamic 사용. 
		// 반드시!!!! UFUNCTION()
		// AddDynamic() 는 반드시 BeginPlay() 이후에 호출해야 합니다.
		Youtuber->OnAirStart.AddDynamic(this, &AMySubscribe::Watch);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("구독자 : 유투버가 지정되지 않음"));
	}
}

// Called every frame
void AMySubscribe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMySubscribe::Watch()
{
	PRINT_LOG(TEXT("구독자 : 방송 시작 알람 받음"));
}

