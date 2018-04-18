// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInfoOnTop.h"

#include "Engine/World.h"

#include "Online/EveryThingGameState_Game.h"


void UGameInfoOnTop::NativeConstruct()
{
	AEveryThingGameState_Game* OwnerETGS_G = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_Game>() : nullptr;
	if (OwnerETGS_G)
	{
		UpdateRemaningGameTime(OwnerETGS_G->GetRemaningGameTime());
		OwnerETGS_G->OnRemaningGameTimeUpdateDelegate.AddUObject(this, &UGameInfoOnTop::UpdateRemaningGameTime);
	}

	Super::NativeConstruct();
}

