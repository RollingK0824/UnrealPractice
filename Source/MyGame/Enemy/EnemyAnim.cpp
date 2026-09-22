// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "MyGame.h"

void UEnemyAnim::OnEndAttackAnimation()
{
	bAttackPlay = false;
	//PRINT_LOG(TEXT("Attack End@@@ %f"), GetWorld()->GetTimeSeconds());
}
