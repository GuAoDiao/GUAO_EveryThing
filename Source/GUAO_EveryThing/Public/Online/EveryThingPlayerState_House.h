// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "EveryThingTypes.h"

#include "EveryThingPlayerState_House.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingPlayerState_House : public APlayerState
{
	GENERATED_BODY()
	
public:
	AEveryThingPlayerState_House();
	virtual void BeginPlay() override;



	virtual void SeamlessTravelTo(class APlayerState* NewPlayerState) override;


	//////////////////////////////////////////////////////////////////////////
	/// PlayerInfo

public:
	const FPlayerInfo& GetPlayerInfo() const { return CurrentPlayerInfo; }
	void SetPlayerInfo(const FPlayerInfo& InPlayerInfo);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoUpdateDelegate, const FPlayerInfo& /* CurrentPlayerInfo */);
	FOnPlayerInfoUpdateDelegate OnPlayerInfoUpdateDelegate;
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerInfo(const FPlayerInfo& InPlayerInfo);

	UFUNCTION()
	void OnRep_CurrentPlayerInfo() { OnCurrentPlayerInfoUpdate(); }
	void OnCurrentPlayerInfoUpdate() { OnPlayerInfoUpdateDelegate.Broadcast(CurrentPlayerInfo); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentPlayerInfo)
	FPlayerInfo CurrentPlayerInfo;
	
	//////////////////////////////////////////////////////////////////////////
	/// Is Ready
public:
	bool GetIsReady() const { return bIsReady; }
	void SetIsReady(bool bInIsReady);
	UFUNCTION(BlueprintCallable)
	void TooggleReadState();
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIsReadyUpdateDelegate, bool /* bIsReady */);
	FOnbIsReadyUpdateDelegate OnbIsReadyUpdateDelegate;
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTooggleReadState();

	UFUNCTION()
	void OnRep_bIsReady() { OnIsReadyUptate(); }
	void OnIsReadyUptate() { OnbIsReadyUpdateDelegate.Broadcast(bIsReady); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_bIsReady)
	bool bIsReady;
	
	//////////////////////////////////////////////////////////////////////////
	/// Team ID
public:
	int32 GetTeamID() const { return TeamID; }
	void SetTeamID(int32 InTeamID) { TeamID = InTeamID; OnTeamIDUpdate(); }
	void ChangeTeamID(int32 InTeamID);
	void OnAllowedTeamNumChanged(int32 AllowedTeamNum);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamIDUpdateDelegate, int32 /* TeamID */);
	FOnTeamIDUpdateDelegate OnTeamIDUpdateDelegate;
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeTeamID(int32 InTeamID);
	

	UFUNCTION()
	void OnRep_TeamID() { OnTeamIDUpdate(); }
	void OnTeamIDUpdate() { OnTeamIDUpdateDelegate.Broadcast(TeamID); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamID)
	int32 TeamID;

	//////////////////////////////////////////////////////////////////////////
	/// House Owner
public:
	UFUNCTION(BlueprintCallable)
	bool CheckIsHouseOwner();
};
