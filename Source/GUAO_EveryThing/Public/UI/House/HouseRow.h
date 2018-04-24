// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"
#include "HouseRow.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UHouseRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeUI(FOnlineSessionSearchResult& InSearchResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeDisplay(const FString& HouseName, const FString& GameType, const FString& MapName, int32 CurrentPlayerNums, int32 MaxPlayerNums, int32 Pin, const FString& State);

	UFUNCTION(BlueprintCallable)
	void UpdateDisplay();

	UFUNCTION(BlueprintCallable)
	void JoinHouse();
private:
	FOnlineSessionSearchResult* SearchResult;
};
