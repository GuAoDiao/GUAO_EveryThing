// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseRow.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "EveryThingGameInstance.h"
#include "UI/EveryThingHUD_Menu.h"

void UHouseRow::InitializeUI(FOnlineSessionSearchResult& InSearchResult)
{
	SearchResult = &InSearchResult;

	UpdateDisplay();
}


void UHouseRow::UpdateDisplay()
{
	if (SearchResult)
	{
		// get house info from SearchResult
 		const FOnlineSessionSettings& Settings = SearchResult->Session.SessionSettings;

		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
		FString GameType, MapName, HouseName, GameState;

		Settings.Get<FString>(SETTING_GAMEMODE, GameType);
		Settings.Get<FString>(SETTING_MAPNAME, MapName);
		Settings.Get<FString>(FName("HouseName"), HouseName);
		Settings.Get<FString>(FName("GameState"), GameState);


		int32 MaxPlayersNum = Settings.NumPublicConnections;
		int32 Pin = SearchResult->PingInMs;
		int32 CurrentPlayersNum = MaxPlayersNum - SearchResult->Session.NumOpenPublicConnections;


		InitializeDisplay(HouseName, GameType, MapName, CurrentPlayersNum, MaxPlayersNum, Pin, GameState);

	}
}


void UHouseRow::JoinHouse()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	if (OwnerPC && SearchResult)
	{
		AEveryThingHUD_Menu* OwnerMenuHUD = Cast<AEveryThingHUD_Menu>(OwnerPC->GetHUD());
		if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::LoadingScreen); }

		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(OwnerPC->GetGameInstance());
		if (OwnerETGI) { OwnerETGI->JoinGame(*SearchResult); }
	}
}