#include "Character/PlayerBaseComponent.h"

UPlayerBaseComponent::UPlayerBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}

void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<ATPSPlayer>(GetOwner());
	MoveComp = Me->GetCharacterMovement();
}

void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

