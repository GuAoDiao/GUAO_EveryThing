// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HouseLayout.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UHouseLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void ExitHouse();

	//////////////////////////////////////////////////////////////////////////
	/// Player Item

	void OnAddPlayer(class APlayerState* PlayerState);
	void OnRemovePlayer(class APlayerState* PlayerState);

	UFUNCTION(BlueprintImplementableEvent)
	void AddHousePlayerItem(class UHousePlayerItem* HousePlayerItem);
	
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveHousePlayerItem(class UHousePlayerItem* HousePlayerItem);

	TMap<APlayerState*, UHousePlayerItem*> AllPlayerItemList;

	//////////////////////////////////////////////////////////////////////////
	/// Player Info And State
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsHouseOwner(bool bIsHouserOwner);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateReadyState(bool bIsReady);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAllowedTeamNum(int32 AllowedTeamNum);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentTeamID(int32 TeamID);

	void OnAllowedTeamNumUpdate(int32 AllowedTeamNum);

	UFUNCTION(BlueprintCallable)
	void ChangeTeamID(int32 ID);

	UFUNCTION(BlueprintCallable)
	void ToggleReadState();

	UFUNCTION(BlueprintCallable)
	void StartGame();
	
protected:
	void OnPlayerStateUpdate(class APlayerState* PlayerState);

	//////////////////////////////////////////////////////////////////////////
	/// House Setting
public:
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllMaps(const FString& MapType) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FString>& GetAllMapsType() const;

	UFUNCTION(BlueprintCallable)
	void UpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum);
};
