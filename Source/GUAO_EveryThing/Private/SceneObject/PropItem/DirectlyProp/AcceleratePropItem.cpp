// Fill out your copyright notice in the Description page of Project Settings.

#include "AcceleratePropItem.h"

#include "TimerManager.h"

#include "Characters/GamePawn.h"

#define LOCTEXT_NAMESPACE "Everything_SceneObject_AcceleratePropItem"

bool AAcceleratePropItem::BeOverlapByGamePawn(class AGamePawn* OverlapGamePawn)
{
	if (OverlapGamePawn && !CurrentEffectGamePawn)
	{
		OverlapGamePawn->SetAgility(OverlapGamePawn->GetAgility() * 3.f);
		CurrentEffectGamePawn = OverlapGamePawn;

		CreatePlayerFightInfoFromProp(CurrentEffectGamePawn, LOCTEXT("OnStartAccelerate", "You Have been accelerated."));

		FTimerDelegate TimerDelegate;
		GetWorldTimerManager().SetTimer(ResetGamePawnAgilityTimer, this, &AAcceleratePropItem::ResetGamePawnAgility, 5.f, false);

		return true;
	}

	return false;
}

void AAcceleratePropItem::ResetGamePawnAgility()
{
	if (CurrentEffectGamePawn)
	{
		CurrentEffectGamePawn->SetAgility(CurrentEffectGamePawn->GetAgility() / 3.f);

		CreatePlayerFightInfoFromProp(CurrentEffectGamePawn, LOCTEXT("OnStopAccelerate", "The speed of acceleration is over."));
		
		CurrentEffectGamePawn = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE