// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Moves/SkillComponent.h"
#include "FootballTestSkillComponent.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UFootballTestSkillComponent : public USkillComponent
{
	GENERATED_BODY()
	
public:
	UFootballTestSkillComponent();

	virtual void StartFirstSkill() override;

	virtual void StartSecondSkill() override;
	virtual void StopSecondSkill() override;
	virtual void ExcuteSecondSkill(float AxisValue) override;

	virtual void StartUltimateSkill() override;

};
