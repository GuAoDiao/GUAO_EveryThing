// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "EveryThingPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
	
public:
	virtual bool CanSpawnGamePawn(AController* Player);
};
