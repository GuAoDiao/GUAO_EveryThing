// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballTestForm.h"

#include "FootballAttackComponent.h"
#include "FootballTestSkillComponent.h"

IMPLEMENT_GAMEPAWNFORM_CLASS("FootballTestForm", FFootballTestForm);

 FFootballTestForm::FFootballTestForm(AGamePawn* InGamePawn) : FRoleForm(InGamePawn)
{
	AttackCompClass = UFootballAttackComponent::StaticClass();
	SkillCompClass = UFootballTestSkillComponent::StaticClass();
}