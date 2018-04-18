// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerFootballPawn.h"


#include "Characters/PlayerPawnComponent.h"

APlayerFootballPawn::APlayerFootballPawn()
{
	OwnerPlayerPawnComponent = CreateDefaultSubobject<UPlayerPawnComponent>(TEXT("PlayerPawnComponent"));
}

UPlayerPawnComponent* APlayerFootballPawn::GetPlayerPawnComponent() const { return OwnerPlayerPawnComponent; }