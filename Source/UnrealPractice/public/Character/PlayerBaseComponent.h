#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBaseComponent.generated.h"


UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class UNREALPRACTICE_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerBaseComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UPROPERTY()
	ATPSPlayer* Me;

	UPROPERTY()
	UCharacterMovementComponent* MoveComp;

	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput) {};
};
