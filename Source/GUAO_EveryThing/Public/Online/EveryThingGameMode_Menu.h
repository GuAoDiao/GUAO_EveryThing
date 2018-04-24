// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EveryThingGameMode_Menu.generated.h"



/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingGameMode_Menu : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AEveryThingGameMode_Menu();

	// do nothing
	virtual void RestartPlayer(class AController* NewPlayer) override {};
};
