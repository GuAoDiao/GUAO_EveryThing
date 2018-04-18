// Fill out your copyright notice in the Description page of Project Settings.

#include "CureProp.h"

#include "Characters/GamePawn.h"

#define LOCTEXT_NAMESPACE "Everything_SceneObject_CureProp"

bool UCureProp::BeUseByGamePawn(AGamePawn* InGamePawn)
{
	if (InGamePawn)
	{
		InGamePawn->ChangeDurability(200.f);

		CreatePlayerFightInfoFromProp(InGamePawn, LOCTEXT("OnUseCureProp", "You've recovered 200 Durability from prop."));

		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE