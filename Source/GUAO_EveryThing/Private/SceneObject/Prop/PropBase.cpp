// Fill out your copyright notice in the Description page of Project Settings.

#include "PropBase.h"

#include "Online/PlayerController_Game.h"
#include "Characters/GamePawn.h"


void UPropBase::CreatePlayerFightInfoFromProp(AGamePawn* TargetGamePawn, const FText& FightInfo)
{
	APlayerController_Game* TargetPC_G = TargetGamePawn ? Cast<APlayerController_Game>(TargetGamePawn->GetController()) : nullptr;
	if (TargetPC_G)
	{
		TargetPC_G->ClientCreatePlayerFightInfo(FightInfo);
	}
}

