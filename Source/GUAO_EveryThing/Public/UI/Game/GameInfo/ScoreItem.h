// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UScoreItem : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	void InitializeScoreItem(class AEveryThingPlayerState_Game* TargetETPS);

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeScoreItemDisplay(const FString& PlayerName, int32 TeamID);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerDeathNumDisplay(int32 DeathCount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerKillNumDisplay(int32 KillCount);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerGameScoreDisplay(int32 GameScore);
};
