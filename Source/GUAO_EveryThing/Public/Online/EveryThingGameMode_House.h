// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EveryThingGameMode_House.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingGameMode_House : public AGameModeBase
{
	GENERATED_BODY()
		
public:
	AEveryThingGameMode_House();


	virtual void BeginPlay() override;
	// do nothing
	virtual void RestartPlayer(class AController* NewPlayer) override {};

	
	bool bToggleToGame;
	void OpenGameFromHouseOwner();
	
	void UpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum);

	virtual void BeginDestroy() override;
};
