// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EveryThingSaveArchivesList.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UEveryThingSaveArchivesList : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> ArchivesList;
};
