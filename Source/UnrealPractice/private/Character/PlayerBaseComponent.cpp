#include "Character/PlayerBaseComponent.h"

UPlayerBaseComponent::UPlayerBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}

void UPlayerBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Me = Cast<ATPSPlayer>(GetOwner());
	MoveComp = Me->GetCharacterMovement();

	Me->OnInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);
}

void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

