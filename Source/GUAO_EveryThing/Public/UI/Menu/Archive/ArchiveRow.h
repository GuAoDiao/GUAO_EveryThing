// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArchiveRow.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UArchiveRow : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeArchiveRow(const FString& InArchiveName);
	
	UFUNCTION(BlueprintCallable)
	void LoadArchive();

private:
	FString ArchiveName;
};
