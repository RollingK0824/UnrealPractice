// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGame.h"
#include "Modules/ModuleManager.h"
#include "Kismet/KismetSystemLibrary.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, MyGame, "MyGame" );
DEFINE_LOG_CATEGORY(MyGame);

// 화면에 출력하는 로그
void PrintScreenLog(int32 Key, float Duration, FColor DisplayColor, const FString& DebugMessage)
{
	// 네트워크 배우면 차이점이 보인다.
	//UKismetSystemLibrary::PrintString(WorldContextObject, Text, true, true, TextColor, Duration);

	GEngine->AddOnScreenDebugMessage(-1, Duration, DisplayColor, DebugMessage);
}