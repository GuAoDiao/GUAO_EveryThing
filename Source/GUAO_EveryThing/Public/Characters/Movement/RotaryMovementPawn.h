// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawn.h"
#include "Characters/Movement/Interfaces/RotaryMovementPawnInterface.h"
#include "RotaryMovementPawn.generated.h"


UCLASS()
class GUAO_EVERYTHING_API ARotaryMovementPawn : public AGamePawn, public IRotaryMovementPawnInterface
{
	GENERATED_BODY()

public:
	ARotaryMovementPawn();

public:
	//////////////////////////////////////////////////////////////////////////
	/// IRotaryMovementPawnInterface

	/// For Controller
	virtual class URotaryMovementComponent* GetRotaryMovementComponent() const override;

	/// For Rotary Movement
	virtual class UPrimitiveComponent* GetPrimitiveComponent() const override;

	virtual const FVector GetActualForwardVector() const override;
	virtual const FVector GetActualRightVector() const override;
	virtual const FVector GetActualUpVector() const override;

	//////////////////////////////////////////////////////////////////////////
	/// Hit
protected:
	virtual void OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit) override;
};