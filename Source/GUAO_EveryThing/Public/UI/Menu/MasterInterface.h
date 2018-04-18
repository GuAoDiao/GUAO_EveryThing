// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterInterface.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UMasterInterface : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Backup();
	
	UFUNCTION(BlueprintCallable)
	void HouseMenu();

	UFUNCTION(BlueprintCallable)
	void Storehouse();
};
