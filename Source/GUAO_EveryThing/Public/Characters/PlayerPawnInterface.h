// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UPlayerPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class GUAO_EVERYTHING_API IPlayerPawnInterface
{
	GENERATED_BODY()

public:
	virtual class UPlayerPawnComponent* GetPlayerPawnComponent() const = 0;
};
