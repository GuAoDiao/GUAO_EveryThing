// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupPropItem.h"

#include "Characters/GamePawn.h"
#include "Characters/PropComponent.h"

bool APickupPropItem::BeOverlapByGamePawn(AGamePawn* OverlapGamePawn)
{
	UPropComponent* PropComp = OverlapGamePawn ? OverlapGamePawn->GetPropComponent() : nullptr;
	if (PickupPropItemID != -1 && PropComp)
	{
		return PropComp->AddProp(PickupPropItemID);
	}

	return false;
}
