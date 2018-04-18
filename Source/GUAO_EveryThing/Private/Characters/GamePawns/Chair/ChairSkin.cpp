// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairSkin.h"

IMPLEMENT_GAMEPAWNSKIN_CLASS("ChairSkin", FChairSkin);

FChairSkin::FChairSkin(UStaticMeshComponent* StaticMeshComp) : FRoleSkin(StaticMeshComp)
{
	MaterialNames.Add(TEXT("ChairMat"));
	MaterialNames.Add(TEXT("ChairWood"));
	MaterialNames.Add(TEXT("ChairTidy"));

	LoadAllGameSkinMaterial();
}