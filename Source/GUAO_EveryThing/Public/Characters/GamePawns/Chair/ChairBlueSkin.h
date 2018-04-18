// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Skin/GamePawnSkin.h"

/**
 * 
 */
class GUAO_EVERYTHING_API FChairBlueSkin : public FRoleSkin
{
public:
	FChairBlueSkin(UStaticMeshComponent* StaticMeshComp);

	DECLARE_GAMEPAWNSKIN_CLASS("ChairBlueSkin");
};
