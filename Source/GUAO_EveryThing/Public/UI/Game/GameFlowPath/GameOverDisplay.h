// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverDisplay.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UGameOverDisplay : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRemaningBackToHouseTime(float RemaningBackToHouseTime);

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeGameOverDisplay(int32 GetGold);
	
};
