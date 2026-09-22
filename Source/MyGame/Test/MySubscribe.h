// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySubscribe.generated.h"

UCLASS()
class MYGAME_API AMySubscribe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySubscribe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 변수로 노출해서 에디터에서 직접 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMyYoutuber* Youtuber;

	// 방송 시작 알람이 오면, 시청한다.
	UFUNCTION()	// 이거 안붙이면, 호출이 안됩니다. 
	void Watch();

};
