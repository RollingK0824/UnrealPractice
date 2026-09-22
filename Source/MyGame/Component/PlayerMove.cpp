// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerMove.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Data/MyDataSubsystem.h"
#include "Data/MyInputDataAsset.h"

UPlayerMove::UPlayerMove()
{
	// Tick 함수 호출되도록 처리
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	// 초기 속도를 걷기로 설정
	moveComp->MaxWalkSpeed = walkSpeed;

}


void UPlayerMove::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerMove::SetupInputBinding(class UEnhancedInputComponent* PlayerInput)
{
	UMyDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem< UMyDataSubsystem>();
	if (DataSubsystem == nullptr)
		return;

	// 게임 플레이 태그를 알아야한다.
	//"Input.Action.Look" -> 에디터에서 이름을 변경하면, 코드도 같이 변경
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_Look), ETriggerEvent::Triggered, this, &UPlayerMove::Input_Look);
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_Move), ETriggerEvent::Triggered, this, &UPlayerMove::Input_Move);
	// 달리기 입력 이벤트 처리 함수 바인딩
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_Run), ETriggerEvent::Started, this, &UPlayerMove::Input_Run);
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_Run), ETriggerEvent::Completed, this, &UPlayerMove::Input_Run);
	// 점프 입력 이벤트 처리 함수 바인딩
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_Jump), ETriggerEvent::Started, this, &UPlayerMove::Input_Jump);
}

void UPlayerMove::Input_Look(const FInputActionValue& inputValue)
{
	const FVector2D Value = inputValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		me->AddControllerYawInput(Value.X);
	}
	if (Value.Y != 0.0f)
	{
		me->AddControllerPitchInput(Value.Y);
	}
}

void UPlayerMove::Input_Move(const struct FInputActionValue& inputValue)
{
	if (me->Controller)
	{
		const FVector2D Value = inputValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, me->Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			me->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			me->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UPlayerMove::Input_Run(const struct FInputActionValue& inputValue)
{
	auto movement = me->GetCharacterMovement();
	// 현재 달리기 모드라면
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		// 걷기 속도로 전환
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}

void UPlayerMove::Input_Jump(const struct FInputActionValue& inputValue)
{
	me->Jump();
}
