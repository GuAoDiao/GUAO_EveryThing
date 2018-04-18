// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameInstance.h"

#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/OnlineSession.h"

#include "MoviePlayer.h"

#include "Online/EveryThingGameMode_Menu.h"
#include "Online/EveryThingGameSession.h"

#include "UI/Interlude.h"

#include "EveryThingAssetManager.h"
#include "EveryThingSaveArchive.h"
#include "EveryThingSaveArchivesList.h"
#include "EveryThingGameViewportClient.h"

UEveryThingGameInstance::UEveryThingGameInstance()
{
	SaveArchiveListName = TEXT("ET_SaveArchiveListName");
}

void UEveryThingGameInstance::Init()
{
	Super::Init();

	// bind delegate to PreLoadMap And  LoadMapFinished
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UEveryThingGameInstance::BeginLoadingMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UEveryThingGameInstance::EndLoadingMap);
}

//////////////////////////////////////////////////////////////////////////
/// Level
void UEveryThingGameInstance::OpenHouseLevel()
{
	UGameplayStatics::OpenLevel(this, HouseLevelName, true, TEXT("listen"));
}

void UEveryThingGameInstance::OpenMenuLevel()
{
	UGameplayStatics::OpenLevel(this, MenuLevelName, true);
}

void UEveryThingGameInstance::ExitGameApplication()
{
	APlayerController* OwnerPC = GetFirstLocalPlayerController();
	if (OwnerPC) { OwnerPC->ConsoleCommand("quit"); }
}


//////////////////////////////////////////////////////////////////////////
/// Loading Map
void UEveryThingGameInstance::BeginLoadingMap(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		LoadingMapMoviePlayer = GetMoviePlayer();
		if (LoadingMapMoviePlayer)
		{
			// create Interlude widget
			if (!Interlude)
			{
				TSubclassOf<UUserWidget> InterludeClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("Interlude");
				Interlude = InterludeClass ? CreateWidget<UInterlude>(this, InterludeClass) : nullptr;
			}

			if(Interlude)
			{
				Interlude->InitializeLoadingMap(this, MapName);

				// set attr and use LoadingScreen.
				FLoadingScreenAttributes LoadingScreenAttributes;
				LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false;
				LoadingScreenAttributes.bMoviesAreSkippable = true;
				LoadingScreenAttributes.bWaitForManualStop = true;
				LoadingScreenAttributes.PlaybackType = EMoviePlaybackType::MT_Looped;
				LoadingScreenAttributes.WidgetLoadingScreen = Interlude->TakeWidget();
				LoadingScreenAttributes.MoviePaths.Add(TEXT("LoadingMap"));

				LoadingMapMoviePlayer->SetupLoadingScreen(LoadingScreenAttributes);
			}
		}
	}
}

bool UEveryThingGameInstance::IsLoadingMapFinished() const
{
	return LoadingMapMoviePlayer ? LoadingMapMoviePlayer->IsLoadingFinished() : true;
}

void UEveryThingGameInstance::EndLoadingMap(UWorld* LoadedWorld)
{
	if (!IsRunningDedicatedServer())
	{
		LoadingMapMoviePlayer = nullptr;

		UEveryThingGameViewportClient* OwnerETGVC = Cast<UEveryThingGameViewportClient>(GetGameViewportClient());
		if (OwnerETGVC) { OwnerETGVC->Fade(5.f, false); }
	}
}

//////////////////////////////////////////////////////////////////////////
/// Match
void UEveryThingGameInstance::HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid()) { OwnerETGS->HostSession(*UserId, NAME_GameSession, HouseName, GameType, MapName, bIsLAN, bIsPresence, MaxPlayersNum); }
	}
}

void UEveryThingGameInstance::FindHoustList(bool bIsLAN, bool bIsPresence)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid()) { OwnerETGS->FindSessions(*UserId, bIsLAN, bIsPresence); }
	}
}

void UEveryThingGameInstance::JoinGame(FOnlineSessionSearchResult& SessionResult)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid() && SessionResult.Session.OwningUserId != UserId)
		{
			OwnerETGS->JoinSession(*UserId, SessionResult);
		}
	}
}

void UEveryThingGameInstance::JoinGame(int32 SearchResultIndex)
{
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	ULocalPlayer* OwnerLocalPlayer = GetFirstGamePlayer();
	if (OwnerETGS && OwnerLocalPlayer)
	{
		TSharedPtr<const FUniqueNetId> UserId = OwnerLocalPlayer->GetPreferredUniqueNetId();
		if (UserId.IsValid()) { OwnerETGS->JoinSession(*UserId, SearchResultIndex); }
	}
}

void UEveryThingGameInstance::ExitGame()
{
	// TODO: Not Implementation
	bool bSuccess = false;
	AEveryThingGameSession* OwnerETGS = GetGameSession();
	if (OwnerETGS)
	{
		bSuccess = OwnerETGS->DestroySession();
	}
	else
	{
		UOnlineSession* OwnerOnlineSession = GetOnlineSession();
		if (OwnerOnlineSession)
		{
			OwnerOnlineSession->EndOnlineSession(NAME_GameSession);
			bSuccess = true;
		}
	}

	if (bSuccess) { OpenMenuLevel(); }
}

inline AEveryThingGameSession* UEveryThingGameInstance::GetGameSession()
{
	AGameModeBase* OwnerGameMode = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;
	return OwnerGameMode ? Cast<AEveryThingGameSession>(OwnerGameMode->GameSession) : nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// Archive

bool UEveryThingGameInstance::CreateArchive(const FString& ArchiveName, const FString& PlayerName)
{
	UEveryThingSaveArchive* CurrentSaveGame = Cast<UEveryThingSaveArchive>(UGameplayStatics::CreateSaveGameObject(UEveryThingSaveArchive::StaticClass()));;
	if (CurrentSaveGame)
	{
		// add default pawn football.
		CurrentSaveGame->PlayerInfo.PlayerName = PlayerName;
		CurrentSaveGame->PlayerInfo.Gold = 2000;
		CurrentSaveGame->PlayerInfo.Experience = 0;

		CurrentSaveGame->PlayerInfo.AllHaveRoleNames.Add("Football");
		CurrentSaveGame->PlayerInfo.AllHaveRoleSkinNames.Add("FootballSkin");
		CurrentSaveGame->PlayerInfo.AllHaveRoleFormNames.Add("FootballForm");

		if (UGameplayStatics::SaveGameToSlot(CurrentSaveGame, ArchiveName, 0))
		{
			CurrentSaveArchiveName = ArchiveName;
			PlayerInfo = CurrentSaveGame->PlayerInfo;
			
			// if save success update archive list
			CurrentArchivesList.Add(ArchiveName);
			UpdateArchivesList();

			return true;
		}
	}

	return false;
}


bool UEveryThingGameInstance::LoadArchiveFromName(const FString& ArchiveName)
{
	UEveryThingSaveArchive* OwnerETSA = Cast<UEveryThingSaveArchive>(UGameplayStatics::LoadGameFromSlot(ArchiveName, 0));
	if (OwnerETSA)
	{
		CurrentSaveArchiveName = ArchiveName;
		SetPlayerInfo(OwnerETSA->PlayerInfo);
		return true;
	}

	return false;
}

bool UEveryThingGameInstance::SaveCurrentArchive() { return SaveArchiveWithName(CurrentSaveArchiveName); }
bool UEveryThingGameInstance::SaveArchiveWithName(const FString& ArchiveName)
{
	UEveryThingSaveArchive* OwnerETSA = Cast<UEveryThingSaveArchive>(UGameplayStatics::CreateSaveGameObject(UEveryThingSaveArchive::StaticClass()));;
	if (OwnerETSA)
	{
		OwnerETSA->PlayerInfo = PlayerInfo;
		if (UGameplayStatics::SaveGameToSlot(OwnerETSA, ArchiveName, 0))
		{
			CurrentSaveArchiveName = ArchiveName;
			return true;
		}
	}

	return false;
}


bool UEveryThingGameInstance::LoadArchivesList()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveArchiveListName, 0))
	{
		UEveryThingSaveArchivesList* OwnerETSAL = Cast<UEveryThingSaveArchivesList>(UGameplayStatics::LoadGameFromSlot(SaveArchiveListName, 0));
		if (OwnerETSAL)
		{
			CurrentArchivesList = OwnerETSAL->ArchivesList;
			return true;
		}
	}
	return false;
}

void UEveryThingGameInstance::UpdateArchivesList()
{
	UEveryThingSaveArchivesList* OwnerETSAL = Cast<UEveryThingSaveArchivesList>(UGameplayStatics::CreateSaveGameObject(UEveryThingSaveArchivesList::StaticClass()));;
	if (OwnerETSAL)
	{
		OwnerETSAL->ArchivesList = CurrentArchivesList;
		bool bSuccess = UGameplayStatics::SaveGameToSlot(OwnerETSAL, SaveArchiveListName, 0);
		
		checkf(bSuccess, TEXT("-_- Save ArchiveList must be success"));
	}
}