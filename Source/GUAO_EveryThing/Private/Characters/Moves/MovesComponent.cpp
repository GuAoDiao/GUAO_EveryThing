// Fill out your copyright notice in the Description page of Project Settings.

#include "MovesComponent.h"

#include "GameFramework/Actor.h"

#include "Characters/GamePawn.h"

UMovesComponent::UMovesComponent()
{
	bReplicates = true;
}

bool UMovesComponent::HasAuthority()
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->HasAuthority() : false;
}

bool UMovesComponent::IsAutonomousProxy()
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->Role == ROLE_AutonomousProxy : false;
}


void UMovesComponent::SetWantedToAcceptHitFunction(bool bWanted)
{
	AGamePawn* OwnerGamePawn = Cast<AGamePawn>(GetOwner());
	if (OwnerGamePawn)
	{
		if (bWanted)
		{
			OnHitDelegateHandle = OwnerGamePawn->OnHitDelegate.AddUObject(this, &UMovesComponent::OnHit);
		}
		else
		{
			OwnerGamePawn->OnHitDelegate.Remove(OnHitDelegateHandle);
		}
	}
}