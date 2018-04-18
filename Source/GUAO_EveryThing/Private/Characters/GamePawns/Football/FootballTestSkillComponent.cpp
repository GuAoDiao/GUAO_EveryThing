// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballTestSkillComponent.h"

UFootballTestSkillComponent::UFootballTestSkillComponent()
{
	FirstSkill.bIsEnableActionPressed = true;

	SecondSkill.bIsEnableActionPressed = true;
	SecondSkill.bIsEnableActionReleased = true;
	SecondSkill.bIsEnableAxis = true;

	UltimateSkill.bIsEnableActionPressed = true;

	RebindAll();
}


void UFootballTestSkillComponent::StartFirstSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football test first skill start"));
}
	 
void UFootballTestSkillComponent::StartSecondSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football test second skill start"));
}
void UFootballTestSkillComponent::StopSecondSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football test second skill stop"));
}
void UFootballTestSkillComponent::ExcuteSecondSkill(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("-_- this is football test second skill excute"));
	}
}
void UFootballTestSkillComponent::StartUltimateSkill()
{
	UE_LOG(LogTemp, Log, TEXT("-_- this is football test ultimate skill start"));
}
