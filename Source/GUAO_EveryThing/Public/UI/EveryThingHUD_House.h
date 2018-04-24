// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EveryThingHUD_House.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingHUD_House : public AHUD
{
	GENERATED_BODY()
	
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(Transient)
	class UHouseLayout* HouseLayout;
};
