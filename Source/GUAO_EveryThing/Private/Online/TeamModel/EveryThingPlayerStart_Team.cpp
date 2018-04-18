// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerStart_Team.h"

#include "Online/PlayerController_Game.h"
#include "Online/EveryThingPlayerState_Game.h"

bool AEveryThingPlayerStart_Team::CanSpawnGamePawn(AController* Player)
{
	APlayerController_Game* PlayerPC_G = Cast<APlayerController_Game>(Player);
	AEveryThingPlayerState_Game* PlayerETPS_G = PlayerPC_G ? Cast<AEveryThingPlayerState_Game>(PlayerPC_G->PlayerState) : nullptr;
	if (PlayerETPS_G && PlayerETPS_G->GetTeamID() == TeamID)
	{
		return Super::CanSpawnGamePawn(Player);
	}

	return false;
}



