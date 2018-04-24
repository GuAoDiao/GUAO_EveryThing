// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateArchive.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UCreateArchive : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Cancel();

	UFUNCTION(BlueprintCallable)
	void CreateArchive(const FString& ArchiveName, const FString& PlayerName);
};
