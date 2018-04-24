// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API USelectItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeSelectItem(const FName& InItemName, bool bHave);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateSelectItemDisplay(bool bHave);	

	UFUNCTION(BlueprintPure)
	const FName& GetItemName() { return ItemName; }
private:
	FName ItemName;
};
