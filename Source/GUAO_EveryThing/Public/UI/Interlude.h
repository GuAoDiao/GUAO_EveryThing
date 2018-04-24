// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interlude.generated.h"

class UEveryThingGameInstance;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UInterlude : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeLoadingMap(UEveryThingGameInstance* InETGY, const FString& MapName);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateLoadingMapDisplay(const FString& MapName);

protected:
	UPROPERTY(BlueprintReadWrite)
	UEveryThingGameInstance* OwnerETGY;
};
