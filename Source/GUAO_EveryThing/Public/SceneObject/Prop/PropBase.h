// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PropBase.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UPropBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool BeUseByGamePawn(class AGamePawn* InGamePawn) { return true; }

protected:
	void CreatePlayerFightInfoFromProp(class AGamePawn* TargetGamePawn, const FText& FightInfo);
};
