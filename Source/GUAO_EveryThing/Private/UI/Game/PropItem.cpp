// Fill out your copyright notice in the Description page of Project Settings.

#include "PropItem.h"

#include "EveryThingAssetManager.h"

void UPropItem::InitializePropItem(int32 InPropID)
{
	PropID = InPropID;
		
	const FPickupPropInfo* PropInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetPropInfoFromID(PropID);
	if (PropInfo)
	{
		InitializePropItemDisplay(*PropInfo);
	}
	else
	{
		EmptyPropItemDisplay();
	}
}
