// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballSkin.h"

IMPLEMENT_GAMEPAWNSKIN_CLASS("FootballSkin", FFootballSkin);

FFootballSkin::FFootballSkin(UStaticMeshComponent* StaticMeshComp) : FRoleSkin(StaticMeshComp)
{
	MaterialNames.Add(TEXT("FootballBlack"));
	MaterialNames.Add(TEXT("FootballWhite"));

	LoadAllGameSkinMaterial();
}