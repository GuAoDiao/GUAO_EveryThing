// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneObject/Prop/PropBase.h"
#include "CureProp.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UCureProp : public UPropBase
{
	GENERATED_BODY()
	
public:	
	virtual bool BeUseByGamePawn(class AGamePawn* InGamePawn) override;

	
};
