// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "EveryThingTypes.h"

#include "EveryThingSaveArchive.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UEveryThingSaveArchive : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPlayerInfo PlayerInfo;	
};