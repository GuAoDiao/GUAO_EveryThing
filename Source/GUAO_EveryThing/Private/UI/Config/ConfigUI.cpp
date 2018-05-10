// Fill out your copyright notice in the Description page of Project Settings.

#include "ConfigUI.h"

#include "UI/EveryThingHUD_Menu.h"

void UConfigUI::Backup()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MainMenu); }
}
