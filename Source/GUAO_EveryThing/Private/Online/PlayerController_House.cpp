// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController_House.h"

#include "Engine/World.h"

#include "Online/EveryThingPlayerState_House.h"
#include "Online/EveryThingGameState_House.h"
#include "Online/EveryThingGameMode_House.h"




void APlayerController_House::StartGameWhenIsHouseOwner()
{
	AEveryThingPlayerState_House* OwnerETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);
	if (!OwnerETPS_H->CheckIsHouseOwner()) { return; }
	
	if (!HasAuthority())
	{
		ServerStartGameWhenIsHouseOwner();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AEveryThingGameState_House* OwnerETGS_H = World->GetGameState<AEveryThingGameState_House>();
		if (OwnerETGS_H && OwnerETGS_H->CheckIsAllPlayerAreReady() && OwnerETGS_H->CheckHaveEnoughTeams())
		{
			AEveryThingGameMode_House* OwnerETGM_H = World->GetAuthGameMode<AEveryThingGameMode_House>();
			if (OwnerETGM_H) { OwnerETGM_H->OpenGameFromHouseOwner(); }
		}
	}
}

bool APlayerController_House::ServerStartGameWhenIsHouseOwner_Validate() { return true; }
void APlayerController_House::ServerStartGameWhenIsHouseOwner_Implementation() { StartGameWhenIsHouseOwner(); }


void APlayerController_House::UpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum)
{
	AEveryThingPlayerState_House* OwnerETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);

	if (!OwnerETPS_H->CheckIsHouseOwner()) { return; }

	if (!HasAuthority())
	{
		ServerUpdateHouseSetting(HouseName, GameType, MapName, bIsLAN, MaxPlayersNum);
		return;
	}
	
	AEveryThingGameMode_House* OwnerETGD_H = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_House>() : nullptr;
	if (OwnerETGD_H)
	{
		OwnerETGD_H->UpdateHouseSetting(HouseName, GameType, MapName, bIsLAN, MaxPlayersNum);
	}
}

bool APlayerController_House::ServerUpdateHouseSetting_Validate(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum) { return true; }
void APlayerController_House::ServerUpdateHouseSetting_Implementation(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum) { UpdateHouseSetting(HouseName, GameType, MapName, bIsLAN, MaxPlayersNum); }


void APlayerController_House::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateUpdateDelegate.Broadcast(PlayerState);
}