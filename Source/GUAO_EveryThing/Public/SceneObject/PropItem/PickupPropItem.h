// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneObject/PropItem/BasePropItem.h"
#include "PickupPropItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API APickupPropItem : public ABasePropItem
{
	GENERATED_BODY()
	
public:	
	virtual bool BeOverlapByGamePawn(class AGamePawn* OverlapGamePawn) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 PickupPropItemID;
};
