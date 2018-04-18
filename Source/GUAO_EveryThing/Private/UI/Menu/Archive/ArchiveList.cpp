// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchiveList.h"

#include "UI/EveryThingHUD_Menu.h"
#include "EveryThingGameInstance.h"


void UArchiveList::NativeConstruct()
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI && OwnerETGI->LoadArchivesList())
	{
		ShowArchiveList(OwnerETGI->GetArchivesList());
	}
}

void UArchiveList::Backup()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MainMenu); }
}