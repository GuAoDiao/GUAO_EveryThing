// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Characters/GamePawnControllerInterface.h"
#include "ChatWindow/ChatWindowControllerInterface.h"

#include "PlayerController_Game.generated.h"

class UAttackComponent;
class USkillComponent;
class UChatComponent;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API APlayerController_Game : public APlayerController, public IGamePawnControllerInterface, public IChatWindowControllerInterface
{
	GENERATED_BODY()
	
public:
	APlayerController_Game();
	
	virtual void BeginPlay() override;	
	virtual void SetPawn(APawn* InPawn) override;
protected:
	void RebindInput();

	void RemoveActionAndAxisBindings(const TArray<FName>& BindingsName);
	void ResetAxisAndActionMapping();

	//////////////////////////////////////////////////////////////////////////
	/// Game flow path
public:
	UFUNCTION(Client, Reliable)
	void ClientWaitForHousePlayerLoad();
	UFUNCTION(Client, Reliable)
	void ClientReadyToStart();
	UFUNCTION(Client, Reliable)
	void ClientStartPlayer();
	UFUNCTION(Client, Reliable)
	void ClientGameOver(int32 Gold);

	//////////////////////////////////////////////////////////////////////////
	/// All Game State Info
public:
	UFUNCTION(Client, Reliable)
	void ClientOnAcceptCure(const FString& CauserName, float Treatment);
	UFUNCTION(Client, Reliable)
	void ClientOnAcceptDamage(const FString& CauserName, float Damage);
	UFUNCTION(Client, Reliable)
	void ClientOnAcceptCriticalDamage(const FString& CauserName);
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAcceptCureDelegate, const FString& /* CauserName */, float /* Treatment */)
	FOnAcceptCureDelegate OnAcceptCureDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAcceptDamageDelegate, const FString& /* CauserName */, float /* Damage */)
	FOnAcceptDamageDelegate OnAcceptDamageDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAcceptCriticalDamageDelegate, const FString& /* CauserName */)
	FOnAcceptCriticalDamageDelegate OnAcceptCriticalDamageDelegate;

	UFUNCTION(Client, Reliable)
	void ClientOnTakeCure(const FString& AccepterName, float Treatment);
	UFUNCTION(Client, Reliable)
	void ClientOnTakeDamage(const FString& AccepterName, float Damage);
	UFUNCTION(Client, Reliable)
	void ClientOnTakeCriticalDamage(const FString& AccepterName);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTakeCureDelegate, const FString& /* AccepterName */, float /* Treatment */);
	FOnTakeCureDelegate OnTakeCureDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTakeDamageDelegate, const FString& /* AccepterName */, float /* Damage */);
	FOnTakeDamageDelegate OnTakeDamageDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeCriticalDamageDelegate, const FString& /* AccepterName */);
	FOnTakeCriticalDamageDelegate OnTakeCriticalDamageDelegate;

	UFUNCTION(Client, Reliable)
	void ClientOnBeKilled(const FString& KillerName);
	UFUNCTION(Client, Reliable)
	void ClientOnSuicided();
	UFUNCTION(Client, Reliable)
	void ClientOnKillOther(const FString& KilledName);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBeKilledDelegate, const FString& /* KillerName */);
	FOnBeKilledDelegate OnBeKilledDelegate;
	DECLARE_MULTICAST_DELEGATE(FOnSuicidedDelegate);
	FOnSuicidedDelegate OnSuicidedDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnKillOtherDelegate, const FString& /* KilledName */);
	FOnKillOtherDelegate OnKillOtherDelegate;

	UFUNCTION(Client, Reliable)
	void ClientOnGamePawnBeKilled(const FString& KilledName, const FString& KillerName);
	UFUNCTION(Client, Reliable)
	void ClientOnGamePawnSuicided(const FString& KilledName);
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGamePawnBeKilledDelegate, const FString& /* KilledName */, const FString& /* KillerName */);
	FOnGamePawnBeKilledDelegate OnGamePawnBeKilledDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGamePawnSuicidedDelegate, const FString& /* KilledName */);
	FOnGamePawnSuicidedDelegate OnGamePawnSuicidedDelegate;

public:
	UFUNCTION(Client, Reliable)
	void ClientCreatePlayerFightInfo(const FText& FightInfo);
	//////////////////////////////////////////////////////////////////////////
	/// Player State
public:
	virtual void OnRep_PlayerState() override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateUpdateDelegate, APlayerState* /* PlayerState */)
	FOnPlayerStateUpdateDelegate OnPlayerStateUpdateDelegate;

	//////////////////////////////////////////////////////////////////////////
	/// For Game Pawn Controller Interface
public:
	virtual AActor* GetAttackTarget() override;

	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window Controller Interface
public:
	virtual UChatComponent* GetChatComponent() const { return ChatComponent; }

private:
	UChatComponent* ChatComponent;

	//////////////////////////////////////////////////////////////////////////
	/// Visual Angle
public:
	void Turn(float AxisValue);
	void LookUp(float AxisValue);	
private:
	class AGamePawn* OwnerGamePawn;
	class UPlayerPawnComponent* OwnerPlayerPawnComp;

	//////////////////////////////////////////////////////////////////////////
	/// UI
protected:
	void DisplayGameMenu();
	void FocusToChatWindow();

	void DisplayScoreBoard();
	void RemoveScoreBoard();

protected:
	class AEveryThingHUD_Game* GetOwnerETHUD_G() const;
	//////////////////////////////////////////////////////////////////////////
	/// Attack and skill
public:	
	void ToggleToNewAttackComponent(UAttackComponent* InAttackComp);
	void ToggleToNewSkillComponent(USkillComponent* InSkillComp);

	//////////////////////////////////////////////////////////////////////////
	/// Toggle Role

public:
	const FName& GetCurrentRoleName() const { return CurrentRoleName; }
	
	void StartToggleRole();
	void StopToggleRole();
	
	void ToggleRoleWithIndex(int32 NumberIndex);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoleNameUpdateDelegate, const FName& /* TargetRoleName */);
	FOnRoleNameUpdateDelegate OnRoleNameUpdateDelegate;
	
private:
	void ToggleRoleWithName(const FName& TargetRoleName);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerToggleRole(const FName& TargetRoleName);

	UFUNCTION()
	void OnRep_CurrentRoleName() { OnCurrentRoleNameUpdate(); }
	void OnCurrentRoleNameUpdate() { OnRoleNameUpdateDelegate.Broadcast(CurrentRoleName); }
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentRoleName)
	FName CurrentRoleName;

	TArray<FName> AllRoleNames;

	//////////////////////////////////////////////////////////////////////////
	/// Delay to restart

public:
	void DelayToRestartGamePawn(float DelayTime);
protected:
	void RestartGamePawn();
};
