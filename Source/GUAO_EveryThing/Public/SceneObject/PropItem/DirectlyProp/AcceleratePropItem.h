// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneObject/PropItem/DirectlyPropItem.h"
#include "AcceleratePropItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AAcceleratePropItem : public ADirectlyPropItem
{
	GENERATED_BODY()

public:
	virtual bool BeOverlapByGamePawn(class AGamePawn* OverlapGamePawn) override;
	
protected:
	void ResetGamePawnAgility();

	AGamePawn* CurrentEffectGamePawn;

	FTimerHandle ResetGamePawnAgilityTimer;
};
