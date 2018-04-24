// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Online/EveryThingGameMode_Game.h"
#include "ETGM_TeamModel.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AETGM_TeamModel : public AEveryThingGameMode_Game
{
	GENERATED_BODY()
	
public:
	AETGM_TeamModel();
	
	virtual bool CanTakeDamage(AGamePawn* PlayerPawn, AActor* OtherActor) override;
	
};
