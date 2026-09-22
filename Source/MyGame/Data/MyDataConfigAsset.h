// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyDataConfigAsset.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UMyDataConfigAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMyInputDataAsset> DA_Input;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class UMyInputDataAsset> DA_Weapon;
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class UMyInputDataAsset> DA_Character;
};
