// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "RotaryMovementPawnInterface.generated.h"

UINTERFACE(MinimalAPI)
class URotaryMovementPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class GUAO_EVERYTHING_API IRotaryMovementPawnInterface
{
	GENERATED_BODY()

public:
	/// For Controller
	virtual class URotaryMovementComponent* GetRotaryMovementComponent() const = 0;

	/// For Rotary Movement
	virtual class UPrimitiveComponent* GetPrimitiveComponent() const = 0;

	virtual const FVector GetActualForwardVector() const = 0;
	virtual const FVector GetActualRightVector() const = 0;
	virtual const FVector GetActualUpVector() const = 0;
};