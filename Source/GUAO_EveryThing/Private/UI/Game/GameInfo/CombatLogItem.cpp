// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatLogItem.h"

#define LOCTEXT_NAMESPACE "EveryThing_GmaeUI_CombatLogItem"

void UCombatLogItem::InitializeCombatLogBeKilled(const FString& KilledGamePawnName, const FString& KillerName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("KillerName"), FText::FromString(KillerName));
	Arguments.Add(TEXT("KilledName"), FText::FromString(KilledGamePawnName));

	FText DisplayInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "{KillerName} Kill {KilledName}."), Arguments);

	InitializeCombatLogItemDisplay(DisplayInfo);
}


void UCombatLogItem::InitializeCombatLogSuicided(const FString& KilledGamePawnName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("KilledName"), FText::FromString(KilledGamePawnName));

	FText DisplayInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "{KilledName} suicided."), Arguments);
	InitializeCombatLogItemDisplay(DisplayInfo);
}


#undef LOCTEXT_NAMESPACE