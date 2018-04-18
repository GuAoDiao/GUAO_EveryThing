// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseList.h"

#include "GameFramework/PlayerController.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"
#include "UI/EveryThingHUD_Menu.h"
#include "UI/House/HouseRow.h"


void UHouseList::FindAllHouseList(bool bIsLAN, bool bIsPresence)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	if (OwnerPC)
	{
		AEveryThingHUD_Menu* OwnerMenuHUD = Cast<AEveryThingHUD_Menu>(OwnerPC->GetHUD());
		if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::LoadingScreen); }

		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(OwnerPC->GetGameInstance());
		if (OwnerETGI) { OwnerETGI->FindHoustList(bIsLAN, bIsPresence); }
	}
}

void UHouseList::UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults)
{
	TSubclassOf<UUserWidget> HouseRowClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("HouseRow"));
	if (HouseRowClass)
	{
		RemoveAllHouseRows();

		for (int32 SearchIndex = 0; SearchIndex < SearchResults.Num(); ++SearchIndex)
		{
			UHouseRow* HouseRow = CreateWidget<UHouseRow>(GetOwningPlayer(), HouseRowClass);
			if (HouseRow)
			{
				HouseRow->InitializeUI(SearchResults[SearchIndex]);
				AddHouseRow(HouseRow);
			}
		}
	}
}

void UHouseList::BackUp()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	// current state may is LoadingScreen or HouseList, only do when state is HouseList
	if (OwnerMenuHUD && OwnerMenuHUD->IsTargetGameUIState(EMenuUIState::HouseList))
	{
		OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::HouseMenu);	
	}
}