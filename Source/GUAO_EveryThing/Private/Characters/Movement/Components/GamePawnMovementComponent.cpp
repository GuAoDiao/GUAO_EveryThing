// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnMovementComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Controller.h"

#include "Characters/GamePawn.h"
#include "Characters/GamePawnStaminaComponent.h"

UGamePawnMovementComponent::UGamePawnMovementComponent()
{
	bReplicates = true;

	MoveForceScale = 0.1f;
	SpeedScale = 0.1f;
	JumpForceScale = 2.f;

	OwnerGamePawn = Cast<AGamePawn>(GetOwner());

	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		checkf(OwnerGamePawn, TEXT("-_- Owner Game Pawn muse be exists."));
	}
}

void UGamePawnMovementComponent::UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale)
{
	ActualMoveForce = Quality * QualityScale * MoveForceScale;
	ActualJumpForce = Agility * Quality * QualityScale * JumpForceScale;
	ActualSpeed = Agility * SpeedScale;
}

bool UGamePawnMovementComponent::AddForceIfHaveEnoughStamina(const FVector& Force)
{
	if (!OwnerGamePawn->GetIsDeath() && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->GetStaminaComp()->CanConsumeForce(Force))
		{
			OwnerPrimitiveComp->AddForce(Force, NAME_None, true);
			OwnerGamePawn->GetStaminaComp()->OnConsumeForce(Force);
			return true;
		}
	}
	return false;
}

bool UGamePawnMovementComponent::AddForceAtLocationIfHaveEnoughStamina(const FVector& Force, const FVector& Location)
{
	if (!OwnerGamePawn->GetIsDeath() && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->GetStaminaComp()->CanConsumeForce(Force))
		{
			OwnerPrimitiveComp->AddForceAtLocation(Force, Location);
			OwnerGamePawn->GetStaminaComp()->OnConsumeForce(Force);
			return true;
		}
	}

	return false;
}

bool UGamePawnMovementComponent::AddTorqueInRadiansIfHaveEnoughStamina(const FVector& Torue)
{
	if (!OwnerGamePawn->GetIsDeath() && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->GetStaminaComp()->CanConsumeTorqueInRadians(Torue))
		{
			OwnerPrimitiveComp->AddTorqueInRadians(Torue, NAME_None, true);
			OwnerGamePawn->GetStaminaComp()->OnConsumeTorqueInRadians(Torue);
			return true;
		}
	}

	return false;
}

bool UGamePawnMovementComponent::AddImpulseIfHaveEnoughStamina(const FVector& Impulse)
{
	if (!OwnerGamePawn->GetIsDeath() && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->GetStaminaComp()->CanConsumeImpulse(Impulse))
		{
			OwnerPrimitiveComp->AddImpulse(Impulse);
			OwnerGamePawn->GetStaminaComp()->OnConsumeImpulse(Impulse);
			return true;
		}
	}
	return false;
}