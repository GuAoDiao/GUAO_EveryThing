// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseCreate.h"

#include "EveryThingTypes.h"
#include "EveryThingAssetManager.h"
#include "EveryThingGameInstance.h"
#include "UI/EveryThingHUD_Menu.h"

void UHouseCreate::HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::LoadingScreen); }

	UEveryThingGameInstance* OwnerGameInstance = GetWorld() ? GetWorld()->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->HostGame(HouseName, GameType, MapName, bIsLAN, bIsPresence, MaxPlayersNum); }
}


void UHouseCreate::BackUp()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD && OwnerMenuHUD->IsTargetGameUIState(EMenuUIState::HouseCreate))
	{
		OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseMenu);
	}
}

TArray<FString> UHouseCreate::GetAllMaps(const FString& MapType) const
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetAllMapsFormType(MapType);
}

const TArray<FString>& UHouseCreate::GetAllMapsType() const
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetAllMapsType();
}