// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnSkin.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "EveryThingAssetManager.h"



FRoleSkin::FRoleSkin(UStaticMeshComponent* StaticMeshComp)
{
	OwnerStaticMeshComp = StaticMeshComp;
}

void FRoleSkin::LoadAllGameSkinMaterial()
{
	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();

	for (int32 i = 0; i < MaterialNames.Num(); ++i)
	{
		AssetManager->NeededMaterialFromName(MaterialNames[i]);
	}
}

void FRoleSkin::LoadGamePawnSkin()
{
	if (OwnerStaticMeshComp)
	{
		UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();

		for (int32 i = 0; i < MaterialNames.Num(); ++i)
		{
			UMaterialInstanceConstant* MaterialInstance = AssetManager->GetMaterialFromName(MaterialNames[i], true);
			if (MaterialInstance) { OwnerStaticMeshComp->SetMaterial(i, MaterialInstance); }
		}
	}
}

void FRoleSkin::UnloadGamePawnSkin()
{

}