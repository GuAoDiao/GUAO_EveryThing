// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "JumpMovementPawnInterface.generated.h"

UINTERFACE(MinimalAPI)
class UJumpMovementPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class GUAO_EVERYTHING_API IJumpMovementPawnInterface
{
	GENERATED_BODY()

public:
	/// For Controller
	virtual class UJumpMovementComponent* GetJumpMovementComponent() const = 0;
	/// For Rotary Movement
	virtual class UPrimitiveComponent* GetPrimitiveComponent() const = 0;
};