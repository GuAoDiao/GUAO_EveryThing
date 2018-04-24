// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePawnMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UGamePawnMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGamePawnMovementComponent();
	
	virtual void RebindInputComp(class UInputComponent* OwnerInputComp) {}

public:
	virtual void UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale = 1.f);
protected:	
	float ActualMoveForce;
	float MoveForceScale;

	float ActualJumpForce;
	float JumpForceScale;

	float ActualSpeed;
	float SpeedScale;

	class AGamePawn* OwnerGamePawn;
	class UPrimitiveComponent* OwnerPrimitiveComp;

	bool AddForceIfHaveEnoughStamina(const FVector& Force);
	bool AddForceAtLocationIfHaveEnoughStamina(const FVector& Force, const FVector& Location);
	bool AddTorqueInRadiansIfHaveEnoughStamina(const FVector& Torue);
	bool AddImpulseIfHaveEnoughStamina(const FVector& Impulse);
};
