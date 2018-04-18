// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawns/Chair/ChairPawn.h"

#include "Characters/PlayerPawnInterface.h"

#include "PlayerChairPawn.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API APlayerChairPawn : public AChairPawn, public IPlayerPawnInterface
{
	GENERATED_BODY()
	
public:
	APlayerChairPawn();

	virtual void Tick(float DeltaTime) override;

	virtual UPlayerPawnComponent* GetPlayerPawnComponent() const override;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UPlayerPawnComponent* OwnerPlayerPawnComponent;
};
