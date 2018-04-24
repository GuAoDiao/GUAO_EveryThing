// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerFightInfoItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UPlayerFightInfoItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializePlayerFightInfoItemDisplay(const FText& FightInfo);

	float RemainingTime;
};
