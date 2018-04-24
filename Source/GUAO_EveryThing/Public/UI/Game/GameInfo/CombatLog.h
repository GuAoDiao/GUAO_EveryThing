// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatLog.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UCombatLog : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void OnGamePawnBeKilled(const FString& KilledName, const FString& KillerName);
	void OnGamePawnSuicided(const FString& KilledName);

	UFUNCTION(BlueprintImplementableEvent)
	void AddCombatLogItem(class UCombatLogItem* CombatLogItem);
};
