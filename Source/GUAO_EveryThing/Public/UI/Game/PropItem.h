// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveryThingTypes.h"
#include "PropItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UPropItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializePropItem(int32 InPropID);
	
	int32 GetPropID() const { return PropID; }

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePropNums(int32 PropNums);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializePropItemDisplay(const FPickupPropInfo& Info);

	UFUNCTION(BlueprintImplementableEvent)
	void EmptyPropItemDisplay();
protected:
	int32 PropID;
};
