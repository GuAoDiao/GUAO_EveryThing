// Fill out your copyright notice in the Description page of Project Settings.

#include "ErrorDialog.h"


#include "EveryThingGameInstance.h"
#include "UI/EveryThingHUD_Menu.h"

void UErrorDialog::Close()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToLastGameUIState(); }
}