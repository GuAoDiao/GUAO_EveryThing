// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HouseList.generated.h"

class UHouseRow;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UHouseList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void FindAllHouseList(bool bIsLAN, bool bIsPresence);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddHouseRow(UHouseRow* HouseRow);
	
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveAllHouseRows();

	void UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults);

	UFUNCTION(BlueprintCallable)
	void BackUp();
};
