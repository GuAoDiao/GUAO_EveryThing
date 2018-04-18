// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballSkillComponent.h"

UFootballSkillComponent::UFootballSkillComponent()
{
	FirstSkill.bIsEnableActionPressed = true;
	SecondSkill.bIsEnableActionPressed = true;

	UltimateSkill.bIsEnableActionPressed = true;
	UltimateSkill.bIsEnableActionReleased = true;
	UltimateSkill.bIsEnableAxis = true;

	RebindAll();
}

void UFootballSkillComponent::StartFirstSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football first skill"));
}

void UFootballSkillComponent::StartSecondSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football second skill"));
}

void UFootballSkillComponent::StartUltimateSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football ultimate skill start"));
}

void UFootballSkillComponent::StopUltimateSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football ultimate skill stop"));
}
void UFootballSkillComponent::ExcuteUltimateSkill(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("-_- this is football ultimate skill excute"));
	}
}