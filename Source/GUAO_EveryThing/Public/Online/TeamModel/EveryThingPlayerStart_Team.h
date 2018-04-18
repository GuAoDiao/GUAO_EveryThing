// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Online/EveryThingPlayerStart.h"
#include "EveryThingPlayerStart_Team.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingPlayerStart_Team : public AEveryThingPlayerStart
{
	GENERATED_BODY()
	
public:
	virtual bool CanSpawnGamePawn(AController* Player) override;

	UPROPERTY(EditInstanceOnly)
	int32 TeamID;
	
	
};
