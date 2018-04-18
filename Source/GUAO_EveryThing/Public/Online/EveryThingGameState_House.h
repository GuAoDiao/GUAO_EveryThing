// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "EveryThingTypes.h"

#include "EveryThingGameState_House.generated.h"

class AEveryThingPlayerState_House;


/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingGameState_House : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//////////////////////////////////////////////////////////////////////////
	/// House Player Info
public:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddPlayerDelegate, APlayerState* /* PlayerState */);
	FOnAddPlayerDelegate OnAddPlayerDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemovePlayerDelegate, APlayerState* /* PlayerState */);
	FOnRemovePlayerDelegate OnRemovePlayerDelegate;

	bool CheckPlayerIsHouseOwner(AEveryThingPlayerState_House* InPlayer) const { return InPlayer == HouseOwner; }
	void SetPlayerHouseOwner(AEveryThingPlayerState_House* InPlayer) { HouseOwner = InPlayer; }

	bool CheckIsAllPlayerAreReady();
	bool CheckHaveEnoughTeams();

	//////////////////////////////////////////////////////////////////////////
	/// House Setting
public:
	DECLARE_MULTICAST_DELEGATE_FiveParams(FOnHouseSettingUpdateDelegate, const FString& /*HouseName*/, const FString& /*GameType*/, const FString& /*MapName*/, bool /*bIsLAN*/, int32 /*MaxPlayersNum*/);
	FOnHouseSettingUpdateDelegate OnHouseSettingUpdateDelegate;
	UFUNCTION()
	void OnRep_HouseSettingUpdate() { OnHouseSettingUpdate(); }
	void OnHouseSettingUpdate();

	void OnGameTypeChanged();


	UPROPERTY(Transient, ReplicatedUsing = OnRep_HouseSettingUpdate)
	FString GameType;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_HouseSettingUpdate)
	FString MapName;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_HouseSettingUpdate)
	FString HouseName;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_HouseSettingUpdate)
	bool bIsLANMatch;	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_HouseSettingUpdate)
	int32 MaxPlayerNum;

	//////////////////////////////////////////////////////////////////////////
	/// Team
public:
	int32 GetAllowedTeamNum() const { return AllowedTeamNum; }
	bool CheckTeamIDIsAllowed(int32 TeamID) const { return TeamID > 0 && TeamID <= AllowedTeamNum; }
	int32 GetRandomTeamID() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllowedTeamNumChangeDelegate, int32 /* MaxTeam */);
	FOnAllowedTeamNumChangeDelegate OnAllowedTeamNumChangeDelegate;

protected:
	UFUNCTION()
	void OnRep_AllowedTeamNum() { OnAllowedTeamNumChanged(); }
	void OnAllowedTeamNumChanged() { OnAllowedTeamNumChangeDelegate.Broadcast(AllowedTeamNum); };
	

	UPROPERTY(Transient, ReplicatedUsing = OnRep_AllowedTeamNum)
	int32 AllowedTeamNum;

	const FMapTypeInfo* MapTypeInfo;

	UPROPERTY(Transient, Replicated)
	AEveryThingPlayerState_House* HouseOwner;
};
