// Fill out your copyright notice in the Description page of Project Settings.

#include "HousePlayerItem.h"


#include "Online/EveryThingPlayerState_House.h"

void UHousePlayerItem::InitializeHousePlayerItem(class AEveryThingPlayerState_House* PlayerState)
{
	checkf(PlayerState, TEXT("-_- AEyeryThingPlayerState_House must exist when house player item create "));

	PlayerState->OnPlayerInfoUpdateDelegate.AddUObject(this, &UHousePlayerItem::OnPlayerInfoUpdate);
	PlayerState->OnbIsReadyUpdateDelegate.AddUObject(this, &UHousePlayerItem::OnbIsReadyUpdate);
	PlayerState->OnTeamIDUpdateDelegate.AddUObject(this, &UHousePlayerItem::OnTeamIDUpdate);

	OnPlayerInfoUpdate(PlayerState->GetPlayerInfo());
	OnbIsReadyUpdate(PlayerState->GetIsReady());
	OnTeamIDUpdate(PlayerState->GetTeamID());
}