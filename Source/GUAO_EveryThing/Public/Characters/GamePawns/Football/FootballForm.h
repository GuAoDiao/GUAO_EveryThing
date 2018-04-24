// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Form/GamePawnForm.h"

class GUAO_EVERYTHING_API FFootballForm : public FRoleForm
{
public:
	FFootballForm(AGamePawn* InGamePawn);

	DECLARE_GAMEPAWNFORM_CLASS("FootballForm");
};