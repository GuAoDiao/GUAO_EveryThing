// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Online.h"
#include "OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameSession.h"
#include "EveryThingGameSession.generated.h"


class FEveryThingOnlineSessionSettings : public FOnlineSessionSettings
{
public:
	FEveryThingOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxPlayersNum = 4);
	virtual ~FEveryThingOnlineSessionSettings() {}
};

class FEveryThingOnlineSearchSettings : public FOnlineSessionSearch
{
public:
	FEveryThingOnlineSearchSettings(bool bSearchingLAN = false, bool bSearchingPresence = false);
	virtual ~FEveryThingOnlineSearchSettings() {}
};

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AEveryThingGameSession();
	

public:
	bool HostSession(const FUniqueNetId& UserId, FName SessionName, const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	void StartSession();
	void FindSessions(const FUniqueNetId& UserId, bool bIsLAN, bool bIsPresence);
	bool JoinSession(const FUniqueNetId& UserId, int32 SessionIndexInSearResults);
	bool JoinSession(const FUniqueNetId& UserId, const FOnlineSessionSearchResult& SearchResult);
	bool DestroySession();
protected:
	virtual void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName InSessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful);

protected:
	IOnlineSessionPtr GetSessionPtr() const;
protected:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	
	TSharedPtr<FEveryThingOnlineSessionSettings> HostSettings;
	TSharedPtr<FOnlineSessionSearch> SearchSettings;
};
