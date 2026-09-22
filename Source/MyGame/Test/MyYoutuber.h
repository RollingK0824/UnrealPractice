// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyYoutuber.generated.h"

// 델리게이트 선언 (C++ 통신)
//DECLARE_DELEGATE(FOnAirStart); // 1개짜리
//DECLARE_MULTICAST_DELEGATE(FOnAirStart); // 여러개짜리
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAirStart); // 블루프린트에서도 사용 가능


UCLASS()
class MYGAME_API AMyYoutuber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyYoutuber();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 델리게이트 변수 선언
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnAirStart OnAirStart;

};
