// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillComponent.h"

USkillComponent::USkillComponent() : FirstSkill(this), SecondSkill(this), UltimateSkill(this)
{
	FirstSkill.BindingName = TEXT("FirstSkill");
	SecondSkill.BindingName = TEXT("SecondSkill");
	UltimateSkill.BindingName = TEXT("UltimateSkill");
}


void USkillComponent::RebindInputComp(UInputComponent* OwnerInputComp)
{
	Super::RebindInputComp(OwnerInputComp);

	FirstSkill.RebindInput(OwnerInputComp);
	SecondSkill.RebindInput(OwnerInputComp);
	UltimateSkill.RebindInput(OwnerInputComp);
}

void USkillComponent::StartFirstSkill() {}
void USkillComponent::StopFirstSkill() {}
void USkillComponent::ExcuteFirstSkill(float AxisValue) {}

void USkillComponent::StartSecondSkill() {}
void USkillComponent::StopSecondSkill() {}
void USkillComponent::ExcuteSecondSkill(float AxisValue) {}

void USkillComponent::StartUltimateSkill() {}
void USkillComponent::StopUltimateSkill() {}
void USkillComponent::ExcuteUltimateSkill(float AxisValue) {}


void USkillComponent::RebindAll()
{
	FirstSkill.RebindFunction((FGamePawnMoves::SkillednessAction)&USkillComponent::StartFirstSkill, (FGamePawnMoves::SkillednessAction)&USkillComponent::StopFirstSkill, (FGamePawnMoves::SkillednessAxis)&USkillComponent::ExcuteFirstSkill);
	SecondSkill.RebindFunction((FGamePawnMoves::SkillednessAction)&USkillComponent::StartSecondSkill, (FGamePawnMoves::SkillednessAction)&USkillComponent::StopSecondSkill, (FGamePawnMoves::SkillednessAxis)&USkillComponent::ExcuteSecondSkill);
	UltimateSkill.RebindFunction((FGamePawnMoves::SkillednessAction)&USkillComponent::StartUltimateSkill, (FGamePawnMoves::SkillednessAction) &USkillComponent::StopUltimateSkill, (FGamePawnMoves::SkillednessAxis)&USkillComponent::ExcuteUltimateSkill);
}
