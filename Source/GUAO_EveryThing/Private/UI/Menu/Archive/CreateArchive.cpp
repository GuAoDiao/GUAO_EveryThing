// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateArchive.h"

#include "UI/EveryThingHUD_Menu.h"
#include "EveryThingGameInstance.h"


void UCreateArchive::Cancel()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MainMenu); }
}

void UCreateArchive::CreateArchive(const FString& ArchiveName, const FString& PlayerName)
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerETGI && OwnerMenuHUD)
	{
		if (OwnerETGI->CreateArchive(ArchiveName, PlayerName))
		{
			OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface);
		}
		else
		{
			OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::ErrorDialog);
			OwnerMenuHUD->SetErrorDialogMessage(TEXT("Can't Create Archive"));
		}
	}
}
