// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(MyGame, Log, All);

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define PRINT_CALLINFO( ) UE_LOG(MyGame, Warning, TEXT("%s"), *CALLINFO)

#define PRINT_LOG(fmt, ...) UE_LOG(MyGame, Warning, TEXT("%s %s"), *CALLINFO, \
*FString::Printf(fmt, ##__VA_ARGS__))

// 화면에 출력하는 로그
void PrintScreenLog(int32 Key, float Duration, FColor DisplayColor, const FString& DebugMessage);


// 커스텀 LineTrace 충돌 채널
#define ECC_WEAPON_TRACE  ECC_GameTraceChannel2