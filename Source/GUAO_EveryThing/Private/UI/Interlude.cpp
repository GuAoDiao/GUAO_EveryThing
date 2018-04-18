// Fill out your copyright notice in the Description page of Project Settings.

#include "Interlude.h"

#include "EveryThingGameInstance.h"

void UInterlude::InitializeLoadingMap_Implementation(UEveryThingGameInstance* InETGY, const FString& MapName)
{
	OwnerETGY = InETGY;

	UpdateLoadingMapDisplay(MapName);
}
