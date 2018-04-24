// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UMovesComponent;
class UInputComponent;

struct GUAO_EVERYTHING_API FGamePawnMoves
{
public:
	typedef void(UMovesComponent::*SkillednessAction)();
	typedef void(UMovesComponent::*SkillednessAxis)(float AxisValue);

	FGamePawnMoves(UMovesComponent* InMovesComp = nullptr);
private:
	UMovesComponent* MovesComp;
public:
	void RebindFunction(SkillednessAction InPressedFunc, SkillednessAction InReleasedFunc, SkillednessAxis InExcuteFunc);
	
	void RebindInput(UInputComponent* OwnerInputComp);
private:

public:
	FName BindingName;

	bool bIsEnableActionPressed;
	bool bIsEnableActionReleased;
	bool bIsEnableAxis;
	SkillednessAction StartSkilledness;
	SkillednessAction StopSkilledness;
	SkillednessAxis ExcuteSkilledness;
};