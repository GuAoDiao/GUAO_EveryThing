// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseMenu.h"

#include "UI/EveryThingHUD_Menu.h"

void UHouseMenu::CreateHouse()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseCreate); }
}

void UHouseMenu::HouseList()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseList); }
}

void UHouseMenu::Backup()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface); }
}