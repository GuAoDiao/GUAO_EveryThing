// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverDisplay.h"

#include "Online/EveryThingGameState_Game.h"

void UGameOverDisplay::NativeConstruct()
{
	AEveryThingGameState_Game* OwnerETGS_G = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_Game>() : nullptr;
	if (OwnerETGS_G)
	{
		UpdateRemaningBackToHouseTime(OwnerETGS_G->GetRemaningBackToHouseTime());
		OwnerETGS_G->OnRemaningBackToHouseTimeUpdateDelegate.AddUObject(this, &UGameOverDisplay::UpdateRemaningBackToHouseTime);
	}


	Super::NativeConstruct();
}


