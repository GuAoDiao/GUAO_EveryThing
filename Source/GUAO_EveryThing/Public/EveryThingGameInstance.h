// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EveryThingTypes.h"
#include "EveryThingGameInstance.generated.h"

class UEveryThingSaveArchive;
class UInterlude;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UEveryThingGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEveryThingGameInstance();

	virtual void Init() override;

	//////////////////////////////////////////////////////////////////////////
	/// Level
public:
	void OpenMenuLevel();
	void OpenHouseLevel();
	void ExitGameApplication();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MenuLevelName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName HouseLevelName;


	//////////////////////////////////////////////////////////////////////////
	/// Loading Map
public:
	void BeginLoadingMap(const FString& MapName);
	void EndLoadingMap(UWorld* LoadedWorld);


	UFUNCTION(BlueprintCallable)
	bool IsLoadingMapFinished() const;
private:
	class IGameMoviePlayer* LoadingMapMoviePlayer;
	UPROPERTY()
	UInterlude* Interlude;

	//////////////////////////////////////////////////////////////////////////
	/// Match
public:
	void HostGame(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxPlayersNum);
	void FindHoustList(bool bIsLAN, bool bIsPresence);
	void JoinGame(FOnlineSessionSearchResult& SessionResult);
	void JoinGame(int32 SearchResultIndex);
	void ExitGame();

private:
	class AEveryThingGameSession* GetGameSession();


	//////////////////////////////////////////////////////////////////////////
	/// Archive
public:
	bool LoadArchivesList();
	void UpdateArchivesList();
	const TArray<FString>& GetArchivesList() { return CurrentArchivesList; }

	bool CreateArchive(const FString& ArchiveName, const FString& PlayerName);
	bool LoadArchiveFromName(const FString& ArchiveName);
	bool SaveArchiveWithName(const FString& ArchiveName);
	bool SaveCurrentArchive();
	
	const FString& GetArchiveName() const { return CurrentSaveArchiveName; }

protected:
	FString CurrentSaveArchiveName;
	FString SaveArchiveListName;
	TArray<FString> CurrentArchivesList;
	//////////////////////////////////////////////////////////////////////////
	/// Player Info
public:
	void AddGold(int32 GetGold) { PlayerInfo.Gold += GetGold; UpdatePlayerInfo(); }

	UFUNCTION(BlueprintPure)
	FPlayerInfo& GetPlayerInfo() { return PlayerInfo; }
	
	void SetPlayerInfo(const FPlayerInfo& InPlayerInfo) { PlayerInfo = InPlayerInfo; UpdatePlayerInfo(); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoUpdateDelegate, const FPlayerInfo&);
	FOnPlayerInfoUpdateDelegate OnPlayerInfoUpdateDelegate;

	void UpdatePlayerInfo() { OnPlayerInfoUpdateDelegate.Broadcast(PlayerInfo); SaveCurrentArchive(); }
protected:
	FPlayerInfo PlayerInfo;
};
