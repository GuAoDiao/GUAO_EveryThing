// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/GamePawns/Football/FootballPawn.h"

#include "Characters/PlayerPawnInterface.h"

#include "PlayerFootballPawn.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API APlayerFootballPawn : public AFootballPawn, public IPlayerPawnInterface
{
	GENERATED_BODY()
	
public:
	APlayerFootballPawn();

	virtual UPlayerPawnComponent* GetPlayerPawnComponent() const override;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UPlayerPawnComponent* OwnerPlayerPawnComponent;
};
