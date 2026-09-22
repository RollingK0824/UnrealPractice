// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 게임 종료시까지 사라지지 않는 유일한 엔진이 만들어주는 싱글톤 객체
 */
UCLASS()
class MYGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public: 

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMyDataConfigAsset> DataConfig;
};
