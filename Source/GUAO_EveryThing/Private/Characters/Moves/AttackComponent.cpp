// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"

UAttackComponent::UAttackComponent() : CommonAttack(this), SpecialAttack(this)
{
	CommonAttack.BindingName = TEXT("CommonAttack");
	SpecialAttack.BindingName = TEXT("SpecialAttack");
}

void UAttackComponent::RebindInputComp(UInputComponent* OwnerInputComp)
{
	Super::RebindInputComp(OwnerInputComp);
	
	CommonAttack.RebindInput(OwnerInputComp);
	SpecialAttack.RebindInput(OwnerInputComp);
}

void UAttackComponent::StartCommonAttack() {}
void UAttackComponent::StopCommonAttack() {}
void UAttackComponent::ExcuteCommonAttack(float AxisValue) {}

void UAttackComponent::StartSpecialAttack() {}
void UAttackComponent::StopSpecialAttack() {}
void UAttackComponent::ExcuteSpecialAttack(float AxisValue) {}


void UAttackComponent::RebindAll()
{
	CommonAttack.RebindFunction((FGamePawnMoves::SkillednessAction)&UAttackComponent::StartCommonAttack, (FGamePawnMoves::SkillednessAction)&UAttackComponent::StopCommonAttack, (FGamePawnMoves::SkillednessAxis)&UAttackComponent::ExcuteCommonAttack);
	SpecialAttack.RebindFunction((FGamePawnMoves::SkillednessAction)&UAttackComponent::StartSpecialAttack, (FGamePawnMoves::SkillednessAction)&UAttackComponent::StopSpecialAttack, (FGamePawnMoves::SkillednessAxis)&UAttackComponent::ExcuteSpecialAttack);
}