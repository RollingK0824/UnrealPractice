// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerBaseComponent.h"

UPlayerBaseComponent::UPlayerBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeComponent() 함수가 호출되기 위한 조건
	bWantsInitializeComponent = true;
}


void UPlayerBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	me = Cast<ATPSPlayer>(GetOwner());
	moveComp = me->GetCharacterMovement();
	// 델리게이트에 처리 함수 등록
	me->onInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);
}

void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

