// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballForm.h"

#include "FootballAttackComponent.h"
#include "FootballSkillComponent.h"

IMPLEMENT_GAMEPAWNFORM_CLASS("FootballForm", FFootballForm);

FFootballForm::FFootballForm(AGamePawn* InGamePawn) : FRoleForm(InGamePawn)
{
	AttackCompClass = UFootballAttackComponent::StaticClass();
	SkillCompClass = UFootballSkillComponent::StaticClass();
}