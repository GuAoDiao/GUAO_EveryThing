// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameSession.h"

#include "Engine/World.h"
#include "OnlineSubsystemSessionSettings.h"

#include "EveryThingGameInstance.h"
#include "UI/EveryThingHUD_Menu.h"
#include "EveryThingAssetManager.h"


DECLARE_LOG_CATEGORY_CLASS(EveryThingOnline, Log, All);

namespace
{
	const FString CustomMatchKeyboard("EveryThing");
}

FEveryThingOnlineSessionSettings::FEveryThingOnlineSessionSettings(bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	NumPublicConnections = MaxPlayersNum;
	NumPrivateConnections = 0;
	bIsLANMatch = bIsLAN;
	bShouldAdvertise = true;
	bAllowJoinInProgress = true;
	bAllowInvites = true;
	bUsesPresence = bIsPresence;
	bAllowJoinViaPresence = true;
	bAllowJoinViaPresenceFriendsOnly = false;
}

FEveryThingOnlineSearchSettings::FEveryThingOnlineSearchSettings(bool bSearchingLAN, bool bSearchingPresence)
{
	bIsLanQuery = bSearchingLAN;
	MaxSearchResults = 10;
	PingBucketSize = 50;
	if (bSearchingPresence) { QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); }
}

AEveryThingGameSession::AEveryThingGameSession()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnCreateSessionComplete);
		OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnStartOnlineGameComplete);
		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnFindSessionsComplete);
		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnJoinSessionComplete);
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AEveryThingGameSession::OnDestroySessionComplete);
	}
}


bool AEveryThingGameSession::HostSession(const FUniqueNetId& UserId, FName InSessionName, const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum)
{
	IOnlineSessionPtr Sessions = GetSessionPtr();
		
	if (Sessions.IsValid())
	{
		HostSettings = MakeShareable(new FEveryThingOnlineSessionSettings(bIsLAN, bIsPresence, MaxPlayersNum));
		if (HostSettings.IsValid())
		{
			HostSettings->Set(SETTING_GAMEMODE, GameType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			HostSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			HostSettings->Set(FName("HouseName"), HouseName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			HostSettings->Set(FName("GameState"), FString(TEXT("Ready")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
				

			HostSettings->Set(SETTING_MATCHING_TIMEOUT, 120.f, EOnlineDataAdvertisementType::ViaOnlineService);
			HostSettings->Set(SETTING_MATCHING_HOPPER, FString("TeamDeathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
			HostSettings->Set(SETTING_SESSION_TEMPLATE_NAME, FString("GameSession"), EOnlineDataAdvertisementType::DontAdvertise);

#if !PLATFORM_SWITCH
			// on switch, we don't have room for this in the session data(and it's not used anyway when searching), so there's no need to add it
			HostSettings->Set(SEARCH_KEYWORDS, CustomMatchKeyboard, EOnlineDataAdvertisementType::ViaOnlineService);
#endif
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			return Sessions->CreateSession(UserId, InSessionName, *HostSettings);
		}
	}

	return false;
}

void AEveryThingGameSession::StartSession()
{
	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
		Sessions->StartSession(NAME_GameSession);
	}
}


void AEveryThingGameSession::FindSessions(const FUniqueNetId& UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSessionPtr Sessions = GetSessionPtr();
	
	if (Sessions.IsValid())
	{
		SearchSettings = MakeShareable(new FEveryThingOnlineSearchSettings(bIsLAN, bIsPresence));
		if (SearchSettings.IsValid())
		{
#if !PLATFORM_SWITCH
			SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, CustomMatchKeyboard, EOnlineComparisonOp::Equals);
#endif
			// Sessions->CancelFindSessions();
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(UserId, SearchSettings.ToSharedRef());
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}


bool AEveryThingGameSession::JoinSession(const FUniqueNetId& UserId, int32 SessionIndexInSearResults)
{
	bool bResult = false;

	if (SearchSettings.IsValid())
	{
		if (SessionIndexInSearResults >= 0 && SessionIndexInSearResults < SearchSettings->SearchResults.Num())
		{
			bResult = JoinSession(UserId, SearchSettings->SearchResults[SessionIndexInSearResults]);
		}
	}

	return bResult;
}

bool AEveryThingGameSession::JoinSession(const FUniqueNetId& UserId, const FOnlineSessionSearchResult& SearchResult)
{
	bool bResult = false;
	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
		bResult = Sessions->JoinSession(UserId, NAME_GameSession, SearchResult);
	}

	return bResult;
}

bool AEveryThingGameSession::DestroySession()
{
	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
		return Sessions->DestroySession(NAME_GameSession);
	}
	return false;
}

void AEveryThingGameSession::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnCreateSessionComplete %s bSuccess : %d"), *InSessionName.ToString(), bWasSuccessful);

	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}

	if (bWasSuccessful)
	{
		StartSession();
	}
	else
	{
		APlayerController* OwnerPC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
		AEveryThingHUD_Menu* OwnerETMH = OwnerPC ? Cast<AEveryThingHUD_Menu>(OwnerPC->GetHUD()) : nullptr;
		if (OwnerETMH)
		{
			OwnerETMH->ToggleToNewGameUIState(EMenuUIState::ErrorDialog);
			OwnerETMH->SetErrorDialogMessage(TEXT("Can't Create session"));
		}
	}
}

void AEveryThingGameSession::OnStartOnlineGameComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnStartOnlineGameComplete bSuccess : %d "), bWasSuccessful);

	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	}

	if (bWasSuccessful)
	{
		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
		if (OwnerETGI)
		{
			OwnerETGI->OpenHouseLevel();
		}
	}
	else
	{
		APlayerController* OwnerPC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
		AEveryThingHUD_Menu* OwnerETMH = OwnerPC ? Cast<AEveryThingHUD_Menu>(OwnerPC->GetHUD()) : nullptr;
		if (OwnerETMH)
		{
			OwnerETMH->ToggleToNewGameUIState(EMenuUIState::ErrorDialog);
			OwnerETMH->SetErrorDialogMessage(TEXT("Can't Start session"));
		}
	}
}

void AEveryThingGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnFindSessionsCOmplete bSuccess: %d"), bWasSuccessful);


	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	}

	if (bWasSuccessful && SearchSettings.IsValid())
	{
		UE_LOG(EveryThingOnline, Log, TEXT("-_- Num Search Results: %d"), SearchSettings->SearchResults.Num());
			
		APlayerController* OwnerPC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
		AEveryThingHUD_Menu* OwnerETMH = OwnerPC ? Cast<AEveryThingHUD_Menu>(OwnerPC->GetHUD()) : nullptr;
		if (OwnerETMH) { OwnerETMH->UpdateHouseList(SearchSettings->SearchResults); }
	}
}

void AEveryThingGameSession::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	bool bWillTravel = false;

	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnjoinSessionComplete %s bSuccess: %d"), *InSessionName.ToString(), static_cast<int32>(Result));

	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

		// get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
		// this is something the blueprint node "JoinSession" does automatically!
		APlayerController* OwnerPC = GetGameInstance() ? GetGameInstance()->GetFirstLocalPlayerController() : nullptr;

		// we need a FString to use ClientTravel and we can let the session interface construct such a String for
		// us by giving him the session name and an empty string. we want to do this, because every Online subsystem uses different TravelURLs
		FString TravelURL;

		if (OwnerPC && Sessions->GetResolvedConnectString(InSessionName, TravelURL))
		{
			// finally call the ClientTravel. if you want, you could print the TravelURL to see how it really looks like
			OwnerPC->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
		}
	}
}

void AEveryThingGameSession::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(EveryThingOnline, Log, TEXT("-_- OnDestroySessionComplete %s bSuccess: %d"), *InSessionName.ToString(), bWasSuccessful);

	IOnlineSessionPtr Sessions = GetSessionPtr();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		HostSettings = nullptr;
	}
}

IOnlineSessionPtr AEveryThingGameSession::GetSessionPtr() const
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	return Subsystem ? Subsystem->GetSessionInterface() : nullptr;
}
