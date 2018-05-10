// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfigUI.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UConfigUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Backup();

	
};
