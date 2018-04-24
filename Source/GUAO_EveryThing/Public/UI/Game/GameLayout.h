// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Online/EveryThingGameState_Game.h"

#include "GameLayout.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UGameLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeGameLayout();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGameUIDisplay(EETGameState GameUIState);

	//////////////////////////////////////////////////////////////////////////
	/// About Role Select
public:
	void DisplaySelectRolesBox();
	void RemoveSelectRolesBox();

	void DisplaySelectSkinsBox();
	void RemoveSelectSkinsBox();
	
	void DisplaySelectFormsBox();
	void RemoveSelectFormsBox();

	//////////////////////////////////////////////////////////////////////////
	/// Player Fight Info
public:
	void ClientCreatePlayerFightInfo(const FText& FightInfo);
public:
	UPROPERTY(BlueprintReadWrite, Transient)
	class USelectRolesBox* SelectRolesBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class USelectSkinsBox* SelectSkinsBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class USelectFormsBox* SelectFormsBox;
	
	UPROPERTY(BlueprintReadWrite, Transient)
	class UChatWindow* ChatWindow;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UGameReadyDisplay* ReadyCountDown;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UGameInfoOnTop* GameInfoOnTop;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UImportantNoticeList* ImportNoteBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UCombatLog* CombatLog;

	UPROPERTY(BlueprintReadWrite, Transient)
	class UPlayerInfoDiaplay* PlayerInfoBox;
	UPROPERTY(BlueprintReadWrite, Transient)
	class UPlayerFightInfo* PlayerFightInfo;
};
