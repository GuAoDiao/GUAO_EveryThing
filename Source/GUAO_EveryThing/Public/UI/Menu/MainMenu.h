// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void CreateArchive();
	
	UFUNCTION(BlueprintCallable)
	void ArchiveList();

	UFUNCTION(BlueprintCallable)
	void ConfigUI();

	UFUNCTION(BlueprintCallable)
	void ExitGame();
};
