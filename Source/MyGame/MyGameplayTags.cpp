// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayTags.h"

namespace MyGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Move, "Input.Action.Move");	// 에디터에 노출될 값
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Look, "Input.Action.Look");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Fire, "Input.Action.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Run, "Input.Action.Run");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Jump, "Input.Action.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_ChangeWeapon, "Input.Action.ChangeWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_SniperAim, "Input.Action.SniperAim");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Interact, "Input.Action.Interact");
}