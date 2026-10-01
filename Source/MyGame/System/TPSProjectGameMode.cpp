#include "System/TPSProjectGameMode.h"
#include "MyGame.h"
#include "TPSProjectGameState.h"

ATPSProjectGameMode::ATPSProjectGameMode()
{
	PRINT_LOG(TEXT("My Log : %s"), TEXT("TPS project!!"));
	GameStateClass = ATPSProjectGameState::StaticClass();
}