// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HitAbleInterface.generated.h"

UINTERFACE()
class GUAO_EVERYTHING_API UHitAbleInterface : public UInterface
{
	GENERATED_BODY()
};

class GUAO_EVERYTHING_API IHitAbleInterface
{
	GENERATED_BODY()

public:
	virtual bool CanBeSelectedToHit(AActor* Selector) const = 0;

	virtual void SetIsSelectedToHit(bool bIsInSelectedToHit) = 0;
	
	virtual void AcceptHitFrom(AActor* OtherActor, FVector NormalInpulse, const FHitResult& Hit) = 0;

	virtual const FString& GetHitAbleActorDisplayName() const = 0;
};
