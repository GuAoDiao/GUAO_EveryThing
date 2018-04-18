// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_House.h"

#include "Engine/World.h"

#include "EveryThingGameSession.h"
#include "OnlineSubsystemSessionSettings.h"

#include "Online/EveryThingPlayerState_House.h"
#include "Online/EveryThingGameState_House.h"
#include "Online/PlayerController_House.h"
#include "UI/EveryThingHUD_House.h"
#include "EveryThingAssetManager.h"

AEveryThingGameMode_House::AEveryThingGameMode_House()
{
	GameSessionClass = AEveryThingGameSession::StaticClass();
	PlayerStateClass = AEveryThingPlayerState_House::StaticClass();
	GameStateClass = AEveryThingGameState_House::StaticClass();
	PlayerControllerClass = APlayerController_House::StaticClass();
	HUDClass = AEveryThingHUD_House::StaticClass();

	bUseSeamlessTravel = true;
}


void AEveryThingGameMode_House::BeginPlay()
{
	Super::BeginPlay();

	AEveryThingGameState_House* OwnerETGS_H = GetGameState<AEveryThingGameState_House>();
	if (OwnerETGS_H)
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
		if (Sessions.IsValid())
		{
			FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions->GetSessionSettings(NAME_GameSession);
			if (OwnerOnlineSessionSetting)
			{
				FString GameType, MapName, HouseName;
				OwnerOnlineSessionSetting->Get<FString>(SETTING_GAMEMODE, GameType);
				OwnerOnlineSessionSetting->Get<FString>(SETTING_MAPNAME, MapName);
				OwnerOnlineSessionSetting->Get<FString>(FName("HouseName"), HouseName);

				OwnerETGS_H->GameType = GameType;
				OwnerETGS_H->MapName = MapName;
				OwnerETGS_H->HouseName = HouseName;
				OwnerETGS_H->bIsLANMatch = OwnerOnlineSessionSetting->bIsLANMatch;
				OwnerETGS_H->MaxPlayerNum = OwnerOnlineSessionSetting->NumPublicConnections;
				OwnerETGS_H->OnHouseSettingUpdate();
			}
		}
	}
}


void AEveryThingGameMode_House::OpenGameFromHouseOwner()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	UWorld* World = GetWorld();
	FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions->GetSessionSettings(NAME_GameSession);

	if (World && OwnerOnlineSessionSetting)
	{
		FString MapName;
		OwnerOnlineSessionSetting->Get<FString>(SETTING_MAPNAME, MapName);

		const FMapInfo* MapInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetMapInfoFromName(FName(*MapName));

		checkf(MapInfo, TEXT("-_- the  map actual path must exist."));

		OwnerOnlineSessionSetting->Set("GameState", FString(TEXT("Gaming")));
		Sessions->UpdateSession(NAME_GameSession, *OwnerOnlineSessionSetting, true);

		bToggleToGame = true;

		World->ServerTravel(FString::Printf(TEXT("%s?listen"), *MapInfo->MapPath));
	}
}

void AEveryThingGameMode_House::UpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (Sessions.IsValid())
	{
		FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions->GetSessionSettings(NAME_GameSession);
		if (OwnerOnlineSessionSetting)
		{
			OwnerOnlineSessionSetting->Set(SETTING_GAMEMODE, GameType);
			OwnerOnlineSessionSetting->Set(SETTING_MAPNAME, MapName);
			OwnerOnlineSessionSetting->Set(FName("HouseName"), HouseName);

			OwnerOnlineSessionSetting->bIsLANMatch = bIsLAN;
			OwnerOnlineSessionSetting->NumPublicConnections = MaxPlayersNum;
			Sessions->UpdateSession(NAME_GameSession, *OwnerOnlineSessionSetting, true);


			AEveryThingGameState_House* OwnerETGS_H = GetGameState<AEveryThingGameState_House>();
			if (OwnerETGS_H)
			{
				OwnerETGS_H->GameType= GameType;
				OwnerETGS_H->MapName = MapName;
				OwnerETGS_H->HouseName = HouseName;
				OwnerETGS_H->bIsLANMatch = bIsLAN;
				OwnerETGS_H->MaxPlayerNum = MaxPlayersNum;
				OwnerETGS_H->OnHouseSettingUpdate();
			}
		}
	}
}

void AEveryThingGameMode_House::BeginDestroy()
{
	if (!bToggleToGame)
	{
		AEveryThingGameSession* OwnerGameSession = Cast<AEveryThingGameSession>(GameSession);
		if (OwnerGameSession) { OwnerGameSession->DestroySession(); }
	}

	Super::BeginDestroy();
}