// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairBlueSkin.h"

IMPLEMENT_GAMEPAWNSKIN_CLASS("ChairBlueSkin", FChairBlueSkin);

FChairBlueSkin::FChairBlueSkin(UStaticMeshComponent* StaticMeshComp) : FRoleSkin(StaticMeshComp)
{
	MaterialNames.Add(TEXT("ChairMat_Blue"));
	MaterialNames.Add(TEXT("ChairWood_Blue"));
	MaterialNames.Add(TEXT("ChairTidy_Blue"));

	LoadAllGameSkinMaterial();
}