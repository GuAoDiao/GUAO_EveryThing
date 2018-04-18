// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatLogItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UCombatLogItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeCombatLogSuicided(const FString& KilledGamePawnName);
	void InitializeCombatLogBeKilled(const FString& KilledGamePawnName, const FString& KillerName);
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeCombatLogItemDisplay(const FText& CombatLog);	
	
};
