// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "HousePlayerItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UHousePlayerItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeHousePlayerItem(class AEveryThingPlayerState_House* PlayerState);
	UFUNCTION(BlueprintImplementableEvent)
	void SetTeamDisplayable(bool bDisplayable);
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInfoUpdate(const FPlayerInfo& PlayerInfo);
	UFUNCTION(BlueprintImplementableEvent)
	void OnbIsReadyUpdate(bool bIsReady);
	UFUNCTION(BlueprintImplementableEvent)
	void OnTeamIDUpdate(int32 TeamID);
};
