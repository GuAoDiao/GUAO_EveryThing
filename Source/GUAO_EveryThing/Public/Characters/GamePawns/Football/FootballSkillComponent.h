// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Moves/SkillComponent.h"
#include "FootballSkillComponent.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UFootballSkillComponent : public USkillComponent
{
	GENERATED_BODY()
	
public:
	UFootballSkillComponent();

	virtual void StartFirstSkill() override;

	virtual void StartSecondSkill() override;

	virtual void StartUltimateSkill() override;
	virtual void StopUltimateSkill() override;
	virtual void ExcuteUltimateSkill(float AxisValue) override;
};
