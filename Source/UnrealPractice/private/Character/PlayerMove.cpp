#include "Character/PlayerMove.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	MoveComp->MaxWalkSpeed = WalkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerMove();
}

void UPlayerMove::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	Me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	Me->AddControllerPitchInput(value);
}

void UPlayerMove::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	direction.X = value.X;
	direction.Y = value.Y;
}

void UPlayerMove::PlayerMove()
{
	direction = FTransform(Me->GetControlRotation()).TransformVector(direction);
	Me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::InputRun()
{
	//auto movement = Me->GetCharacterMovement();

	if (MoveComp->MaxWalkSpeed > WalkSpeed)
	{	
		MoveComp->MaxWalkSpeed = WalkSpeed;
	}	
	else
	{	
		MoveComp->MaxWalkSpeed = RunSpeed;
	}
}


void UPlayerMove::InputJump(const struct FInputActionValue& inputValue)
{
	Me->Jump();
}

void UPlayerMove::SetupInputBinding(UEnhancedInputComponent* playerInput)
{
	playerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &UPlayerMove::Turn);
	playerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &UPlayerMove::LookUp);
	playerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UPlayerMove::Move);
	playerInput->BindAction(IA_PlayerRun, ETriggerEvent::Started, this, &UPlayerMove::InputRun);
	playerInput->BindAction(IA_PlayerRun, ETriggerEvent::Completed, this, &UPlayerMove::InputRun);
	playerInput->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &UPlayerMove::InputJump);
}