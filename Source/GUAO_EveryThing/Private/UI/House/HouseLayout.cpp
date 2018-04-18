// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseLayout.h"

#include "EveryThingAssetManager.h"
#include "UI/House/HousePlayerItem.h"
#include "Online/EveryThingGameState_House.h"
#include "Online/EveryThingPlayerState_House.h"
#include "Online/PlayerController_House.h"
#include "EveryThingGameInstance.h"

void UHouseLayout::NativeConstruct()
{
	Super::NativeConstruct();

	AEveryThingGameState_House* OwnerETGS_H = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_House>() : nullptr;
	if (OwnerETGS_H)
	{
		for (APlayerState* PlayerState : OwnerETGS_H->PlayerArray)
		{
			OnAddPlayer(PlayerState);
		}

		OwnerETGS_H->OnAddPlayerDelegate.AddUObject(this, &UHouseLayout::OnAddPlayer);
		OwnerETGS_H->OnRemovePlayerDelegate.AddUObject(this, &UHouseLayout::OnRemovePlayer);
		OwnerETGS_H->OnHouseSettingUpdateDelegate.AddUObject(this, &UHouseLayout::OnUpdateHouseSetting);
		OwnerETGS_H->OnAllowedTeamNumChangeDelegate.AddUObject(this, &UHouseLayout::OnAllowedTeamNumUpdate);

		// TODO : this is only implementation with the simple methods but may be a bug
		OwnerETGS_H->OnHouseSettingUpdate();
		OnAllowedTeamNumUpdate(OwnerETGS_H->GetAllowedTeamNum());
	}

	APlayerController_House* OwnerPC_H = Cast<APlayerController_House>(GetOwningPlayer());
	if (OwnerPC_H)
	{
		OnPlayerStateUpdate(OwnerPC_H->PlayerState);
		OwnerPC_H->OnPlayerStateUpdateDelegate.AddUObject(this, &UHouseLayout::OnPlayerStateUpdate);
	}
}

void UHouseLayout::OnPlayerStateUpdate(class APlayerState* PlayerState)
{
	AEveryThingPlayerState_House* OwnerETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);
	if (OwnerETPS_H)
	{
		UpdateIsHouseOwner(OwnerETPS_H->CheckIsHouseOwner());
		UpdateReadyState(OwnerETPS_H->GetIsReady());
		UpdateCurrentTeamID(OwnerETPS_H->GetTeamID());
		OwnerETPS_H->OnbIsReadyUpdateDelegate.AddUObject(this, &UHouseLayout::UpdateReadyState);
		OwnerETPS_H->OnTeamIDUpdateDelegate.AddUObject(this, &UHouseLayout::UpdateCurrentTeamID);
	}
}

void UHouseLayout::OnAddPlayer(class APlayerState* PlayerState)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	TSubclassOf<UUserWidget> HousePlayerItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("HousePlayerItem");
	AEveryThingPlayerState_House* NewETPS = Cast<AEveryThingPlayerState_House>(PlayerState);
	if (OwnerPC && HousePlayerItemClass && NewETPS)
	{
		UHousePlayerItem* HousePlayerItem = CreateWidget<UHousePlayerItem>(OwnerPC, HousePlayerItemClass);
		if (HousePlayerItem)
		{
			HousePlayerItem->InitializeHousePlayerItem(NewETPS);
			AddHousePlayerItem(HousePlayerItem);

			AllPlayerItemList.Add(PlayerState, HousePlayerItem);
		}
	}
}

void UHouseLayout::OnRemovePlayer(class APlayerState* PlayerState)
{
	if (AllPlayerItemList.Contains(PlayerState))
	{
		RemoveHousePlayerItem(AllPlayerItemList[PlayerState]);

		AllPlayerItemList.Remove(PlayerState);
	}
}



void UHouseLayout::ToggleReadState()
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingPlayerState_House* OwnerETPS_H = OwnerPC ? Cast<AEveryThingPlayerState_House>(OwnerPC->PlayerState) : nullptr;
	if (OwnerETPS_H)
	{
		OwnerETPS_H->TooggleReadState();
	}
}



void UHouseLayout::OnAllowedTeamNumUpdate(int32 AllowedTeamNum)
{
	UpdateAllowedTeamNum(AllowedTeamNum);

	bool bHasTeam = AllowedTeamNum >= 2;

	for (TMap<APlayerState*, UHousePlayerItem*>::TIterator It(AllPlayerItemList); It; ++It)
	{
		if (It.Value())
		{
			It.Value()->SetTeamDisplayable(bHasTeam);
		}
	}

	if (bHasTeam)
	{
		APlayerController* OwnerPC = GetOwningPlayer();
		AEveryThingPlayerState_House* OwnerETPS_H = OwnerPC ? Cast<AEveryThingPlayerState_House>(OwnerPC->PlayerState) : nullptr;
		if (OwnerETPS_H)
		{
			// may current TeamId not update in time. so do check
			int32 CurrentTeamID = OwnerETPS_H->GetTeamID();
			if (CurrentTeamID > 0 && CurrentTeamID <= AllowedTeamNum)
			{
				UpdateCurrentTeamID(CurrentTeamID);
			}
		}
	}
}

void UHouseLayout::ChangeTeamID(int32 ID)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingPlayerState_House* OwnerETPS_H = OwnerPC ? Cast<AEveryThingPlayerState_House>(OwnerPC->PlayerState) : nullptr;
	if (OwnerETPS_H)
	{
		OwnerETPS_H->ChangeTeamID(ID);
	}
}

void UHouseLayout::StartGame()
{
	APlayerController_House* OwnerPC_H = Cast<APlayerController_House>(GetOwningPlayer());
	if (OwnerPC_H) { OwnerPC_H->StartGameWhenIsHouseOwner(); }
}




TArray<FString> UHouseLayout::GetAllMaps(const FString& MapType) const
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetAllMapsFormType(MapType);
}

const TArray<FString>& UHouseLayout::GetAllMapsType() const
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetAllMapsType();
}

void UHouseLayout::UpdateHouseSetting(const FString& HouseName, const FString& GameType, const FString& MapName, bool bIsLAN, int32 MaxPlayersNum)
{
	APlayerController_House* OwnerPC_H = Cast<APlayerController_House>(GetOwningPlayer());
	if (OwnerPC_H)
	{
		OwnerPC_H->UpdateHouseSetting(HouseName, GameType, MapName, bIsLAN, MaxPlayersNum);
	}
}



void UHouseLayout::ExitHouse()
{
	UEveryThingGameInstance* OwnerGameInstance = GetWorld() ? GetWorld()->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->OpenMenuLevel(); }
}