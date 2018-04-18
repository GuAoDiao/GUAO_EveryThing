// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArchiveList.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UArchiveList : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowArchiveList(const TArray<FString>& ArchiveList);

	UFUNCTION(BlueprintCallable)
	void Backup();
};
