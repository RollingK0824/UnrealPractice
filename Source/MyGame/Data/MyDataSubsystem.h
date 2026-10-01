// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyGameplayTags.h"
#include "MyDataSubsystem.generated.h"

/**
 * 엔진이 생성해주는 싱글톤 클래스. 우리가 new(X) 엔진이 게임시작하면, 알아서 생성, 알아서 파괴
 * GameInstance와 동일하지만, GameInstance가 너무 커지는걸 방지하기 위해서
 * 로직별로 Subsystem을 활용해서 설계하면 유지보수에 좋다.
 */

class UInputAction;

UCLASS()
class MYGAME_API UMyDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindInputActionByTag(const FGameplayTag& InputTag) const; 
};
