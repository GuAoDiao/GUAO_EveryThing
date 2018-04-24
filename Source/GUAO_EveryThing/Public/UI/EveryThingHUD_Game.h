// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "EveryThingTypes.h"
#include "ChatWindow/ChatWindowHUDInterface.h"

#include "EveryThingHUD_Game.generated.h"

class UChatWindow;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingHUD_Game : public AHUD, public IChatWindowHUDInterface
{
	GENERATED_BODY()
	
public:
	AEveryThingHUD_Game();

	virtual void BeginPlay() override;


	//////////////////////////////////////////////////////////////////////////
	/// Main Widget
protected:
	UPROPERTY(Transient)
	class UGameLayout* GameLayout;

	//////////////////////////////////////////////////////////////////////////
	/// Game UI State
public:
	EETGameState GetGameUIState() const { return CurrentGameUIState; }
	void ToggleToTargetGameUIState(EETGameState InGameUIState);

	void ShowGameOver(int32 GetGold);
protected:
	EETGameState CurrentGameUIState;
	UPROPERTY(Transient)
	class UGameOverDisplay* GameOver;
	//////////////////////////////////////////////////////////////////////////
	/// For Chat Window HUD Interface
public:
	/// Initialize 
	virtual void InitializeChatWindow()override;

	virtual void FocusToChatWindow() override;
	virtual void RemoveChatWidnowFocus() override;

	/// Chat Input
	virtual void FocusToChatInput() override;
	virtual void FocusToChatReply() override;
	virtual void FocusToChatCommand() override;

	virtual TSubclassOf<class UUserWidget> GetChatLineWidgetClass()override;
	virtual TSubclassOf<class UUserWidget> GetChatWindowWidgetClass()override;

	// virtual void SetCurrentChannel(EChatChannel::Type ChatChannelType) = 0;

	virtual void ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)override;

protected:
	UPROPERTY(Transient)
	UChatWindow* ChatWindow;

	//////////////////////////////////////////////////////////////////////////
	/// Game Menu
public:
	void ToggleGameMenu();

	UFUNCTION(BlueprintCallable)
	void DisplayGameMenu();
	UFUNCTION(BlueprintCallable)
	void RemoveGameMenu();

protected:
	UPROPERTY(Transient)
	class UGameMenu* GameMenu;

	//////////////////////////////////////////////////////////////////////////
	/// Score Box
public:
	void DisplayScoreBoard();
	void RemoveScoreBoard();

protected:
	UPROPERTY(Transient)
	class UScoreBoard* ScoreBoard;

	//////////////////////////////////////////////////////////////////////////
	/// Player Fight Info
public:
	void ClientCreatePlayerFightInfo(const FText& FightInfo);

	//////////////////////////////////////////////////////////////////////////
	/// Select About Roles 
public:
	void ToggleSelectRolesBox(bool bIsDisplay);
	void ToggleSelectSkinsBox(bool bIsDisplay);
	void ToggleSelectFormsBox(bool bIsDisplay);
};
