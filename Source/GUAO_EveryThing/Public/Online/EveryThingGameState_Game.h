// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "EveryThingTypes.h"
#include "ChatWIndow/ChatWindowGameStateInterface.h"

#include "EveryThingGameState_Game.generated.h"


class AEveryThingPlayerState_Game;
class AGamePawn;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingGameState_Game : public AGameStateBase, public IChatWindowGameStateInterface
{
	GENERATED_BODY()
	
public:
	AEveryThingGameState_Game();


	//////////////////////////////////////////////////////////////////////////
	/// Game Init
public:
	void InitializeETGameState(const FString& InGameType, const FString& InMapName, const FString& InHouseName, bool bInIsLANMatach, int32 InMaxPlayerNum, int32 InCurrentPlayerNum);
	void OnGameTypeChanged();
	//////////////////////////////////////////////////////////////////////////
	/// Player Login/Logout
public:
	void OnPlayerPostLogin(APlayerController* NewPlayer);
	void OnPlayerLogout(AController* Exiting);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateAddDelegate, APlayerState* /* PlayerState */)
	FOnPlayerStateAddDelegate OnPlayerStateAddDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateRemoveDelegate, APlayerState* /* PlayerState */)
	FOnPlayerStateRemoveDelegate OnPlayerStateRemoveDelegate;
	//////////////////////////////////////////////////////////////////////////
	/// Ready
public:
	void StartReadyCountDown();
	float GetRemaningReadyTime() const { return RemaningReadyTime; }
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemaningReadyTimeUpdateDelegate, float /*RemaningReadyTime*/)
	FOnRemaningReadyTimeUpdateDelegate OnRemaningReadyTimeUpdateDelegate;

protected:
	void ReadyCountDown();

	UFUNCTION()
	void OnRep_RemaningReadyTime() { OnRemaningReadyTimeUpdate(); }
	void OnRemaningReadyTimeUpdate() { OnRemaningReadyTimeUpdateDelegate.Broadcast(RemaningReadyTime); }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RemaningReadyTime)
	float RemaningReadyTime;
	float DefaultReadyTime;

	//////////////////////////////////////////////////////////////////////////
	/// Gameing
public:
	void StartGameTimeCountDown();

	float GetRemaningGameTime() const { return RemaningGameTime; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemaningGameTimeUpdateDelegate, float /* RemaningGameTime */)
	FOnRemaningGameTimeUpdateDelegate OnRemaningGameTimeUpdateDelegate;
protected:
	void GameTimeCountDown();
	UFUNCTION()
	void OnRep_RemaningGameTime() { OnRemaningGameTimeUpdate(); }
	void OnRemaningGameTimeUpdate() { OnRemaningGameTimeUpdateDelegate.Broadcast(RemaningGameTime); }
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RemaningGameTime)
	float RemaningGameTime;
	float DefaultGameTime;

	//////////////////////////////////////////////////////////////////////////
	/// Back to house
public:
	void StartBackToHouseTimeCountDown();
	float GetRemaningBackToHouseTime() const { return RemaningBackToHouseTime; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemaningBackToHouseTimeUpdateDelegate, float /* RemaningGameTime */)
	FOnRemaningBackToHouseTimeUpdateDelegate OnRemaningBackToHouseTimeUpdateDelegate;
protected:
	void BackToHouseeTimeCountDown();
	UFUNCTION()
	void OnRep_RemaningBackToHouseTime() { OnRemaningBackToHouseTimeUpdate(); }
	void OnRemaningBackToHouseTimeUpdate() { OnRemaningBackToHouseTimeUpdateDelegate.Broadcast(RemaningBackToHouseTime); }
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RemaningGameTime)
	float RemaningBackToHouseTime;
	float DefaultBackToHouseTime;
	
protected:
	FTimerHandle CountDownTimer;

	//////////////////////////////////////////////////////////////////////////
	/// Game State Attr

public:
	int32 GetActualTeamNums() const { return ActualTeamNums; }


public:
	UPROPERTY(Transient, Replicated)
	FString GameType;
	UPROPERTY(Transient, Replicated)
	FString MapName;
	UPROPERTY(Transient, Replicated)
	FString HouseName;
	UPROPERTY(Transient, Replicated)
	bool bIsLANMatach;
	UPROPERTY(Transient, Replicated)
	int32 MaxPlayerNum;
	UPROPERTY(Transient, Replicated)
	int32 CurrentPlayerNum;

protected:
	int32 HousePlayerNum;
	UPROPERTY(Transient, Replicated)
	int32 ActualTeamNums;
	const FMapTypeInfo* MapTypeInfo;

	float CureScoreScale;
	float DamageScoreScale;
	float CriticalDamageScore;
	float KillScore;
	
	//////////////////////////////////////////////////////////////////////////
	/// Team

public:
	int32 GetAllowedTeamNum() const { return AllowedTeamNum; }
	bool CheckTeamIDIsAllowed(int32 TeamID) const { return TeamID > 0 && TeamID <= AllowedTeamNum; }

	int32 GetRandomTeamID() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllowedTeamNumChangeDelegate, int32 /* MaxTeam */);
	FOnAllowedTeamNumChangeDelegate OnAllowedTeamNumChangeDelegate;
protected:
	void OnAllowedTeamNumChanged() { OnAllowedTeamNumChangeDelegate.Broadcast(AllowedTeamNum); };
	
	UPROPERTY(Transient, Replicated)
	int32 AllowedTeamNum;


	//////////////////////////////////////////////////////////////////////////
	/// Game State
public:
	bool IsTargetETGameState(EETGameState TargetGameStae) { return CurrentETGameState == TargetGameStae; }
	EETGameState GetETGameState() const {return CurrentETGameState;}
	
	void ToggleToTargetETGameState(EETGameState TargetGameStae);

	bool IsETGameStarted() const {return bIsETGameStarted;}
protected:
	UPROPERTY(Transient, Replicated)
	EETGameState CurrentETGameState;
	UPROPERTY(Transient, Replicated)
	bool bIsETGameStarted;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Damage And Death
public:
	virtual void OnGamePawnAcceptCure(AGamePawn* AccpetPawn, AActor* Causer, float Treatment);
	virtual void OnGamePawnAcceptDamage(AGamePawn* AccpetPawn, AActor* Causer, float Damage);
	virtual void OnGamePawnAcceptCriticalDamage(AGamePawn* AccpetPawn, AActor* Causer);
	virtual void OnGamePawnBeKilled(AGamePawn* KilledGamePawn, AActor* KillerActor);


	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGamePawnAcceptCureDelegate, AGamePawn* /* AccpetPawn */, AActor* /* Causer */, float /* Cure */)
	FOnGamePawnAcceptCureDelegate OnGamePawnAcceptCureDelegate;
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGamePawnAcceptDamageDelegate, AGamePawn* /* AccpetPawn */, AActor* /* Causer */, float /* Damage */)
	FOnGamePawnAcceptDamageDelegate OnGamePawnAcceptDamageDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGamePawnAcceptCriticalDamageDelegate, AGamePawn* /* AccpetPawn */, AActor* /* Causer */)
	FOnGamePawnAcceptCriticalDamageDelegate OnGamePawnAcceptCriticalDamageDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGamePawnBeKilledDelegate, AGamePawn* /* KilledGamePawn */, AActor* /* KillerActor */)
	FOnGamePawnBeKilledDelegate OnGamePawnBeKilledDelegate;

	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window Game State Interface
public:
	virtual const FString& GetPlayerChatName(int32 PlayerID) const override;
	virtual const TArray<APlayerState*>& GetAllChatPlayerState() const override { return ChatPlayerState; }
	
	const static FString PlayerChatName_NONE;

	int32 GetNextChatID(APlayerState* PlayerState) { return ChatPlayerState.Add(PlayerState);}
protected:
	UPROPERTY(Transient, Replicated)
	TArray<APlayerState*> ChatPlayerState;
};
