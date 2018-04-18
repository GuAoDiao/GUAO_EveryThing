// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnMovementComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Controller.h"

#include "Characters/GamePawn.h"

UGamePawnMovementComponent::UGamePawnMovementComponent()
{
	bReplicates = true;

	MoveForceScale = 0.1f;
	SpeedScale = 0.1f;
	JumpForceScale = 2.f;

	OwnerGamePawn = Cast<AGamePawn>(GetOwner());
}

//////////////////////////////////////////////////////////////////////////
/// Authority	
bool UGamePawnMovementComponent::HasAuthority()
{
	return OwnerGamePawn ? OwnerGamePawn->HasAuthority() : false;
}

bool UGamePawnMovementComponent::IsAutonomousProxy()
{
	return OwnerGamePawn ? OwnerGamePawn->Role == ROLE_AutonomousProxy : false;
}
bool UGamePawnMovementComponent::IsLocalPlayer()
{
	AController* OwnerController = OwnerGamePawn ? OwnerGamePawn->GetController() : nullptr;
	return OwnerController ? OwnerController->IsLocalController() : false;
}


void UGamePawnMovementComponent::UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale)
{
	ActualMoveForce = Quality * QualityScale * MoveForceScale;
	ActualJumpForce = Agility * Quality * QualityScale * JumpForceScale;
	ActualSpeed = Agility * SpeedScale;
}

bool UGamePawnMovementComponent::AddForceIfHaveEnoughStamina(const FVector& Force)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeForce(Force))
		{
			OwnerPrimitiveComp->AddForce(Force, NAME_None, true);
			OwnerGamePawn->OnConsumeForce(Force);
			return true;
		}
	}
	return false;
}

bool UGamePawnMovementComponent::AddForceAtLocationIfHaveEnoughStamina(const FVector& Force, const FVector& Location)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeForce(Force))
		{
			OwnerPrimitiveComp->AddForceAtLocation(Force, Location);
			OwnerGamePawn->OnConsumeForce(Force);
			return true;
		}
	}

	return false;
}

bool UGamePawnMovementComponent::AddTorqueInRadiansIfHaveEnoughStamina(const FVector& Torue)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeTorqueInRadians(Torue))
		{
			OwnerPrimitiveComp->AddTorqueInRadians(Torue, NAME_None, true);
			OwnerGamePawn->OnConsumeTorqueInRadians(Torue);
			return true;
		}
	}

	return false;
}

bool UGamePawnMovementComponent::AddImpulseIfHaveEnoughStamina(const FVector& Impulse)
{
	if (OwnerGamePawn && OwnerPrimitiveComp)
	{
		if (OwnerGamePawn->CanConsumeImpulse(Impulse))
		{
			OwnerPrimitiveComp->AddImpulse(Impulse);
			OwnerGamePawn->OnConsumeImpulse(Impulse);
			return true;
		}
	}
	return false;
}