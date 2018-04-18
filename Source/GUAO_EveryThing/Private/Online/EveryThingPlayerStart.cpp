// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerStart.h"

#include "Kismet/KismetSystemLibrary.h"


bool AEveryThingPlayerStart::CanSpawnGamePawn(AController* Player)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	TArray<AActor*> OverlapActors;
	if (UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), 100.f, ObjectTypeQuery, nullptr, TArray<AActor*>(), OverlapActors))
	{
		for (AActor* OverlapActor : OverlapActors)
		{
			if (OverlapActor && OverlapActor->GetActorEnableCollision())
			{
				return false;
			}
		}
	}

	return true;
}
