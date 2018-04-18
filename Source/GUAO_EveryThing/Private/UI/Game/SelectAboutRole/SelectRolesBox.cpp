// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectRolesBox.h"

#include "Online/EveryThingPlayerState_Game.h"
#include "Online/PlayerController_Game.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "UI/Game/SelectAboutRole/SelectItem.h"

void USelectRolesBox::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* OwnerPC = GetOwningPlayer();

	TSubclassOf<UUserWidget> SelectItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("SelectItem");
	if (SelectItemClass)
	{
		const TMap<FName, FRoleInfo>& AllRolesInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetAllRolesInfo();
		for (TMap<FName, FRoleInfo>::TConstIterator It(AllRolesInfo); It; ++It)
		{
			USelectItem* SelectItem = CreateWidget<USelectItem>(OwnerPC, SelectItemClass);
			if (SelectItem)
			{
				const FRoleInfo& RoleInfo = It.Value();
				SelectItem->InitializeSelectItem(RoleInfo.Name, false);
				AddSelectItem(SelectItem);
			}
		}
	}

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(OwnerPC);
	if (OwnerPC_G)
	{
		OnPlayerStateUpdate(OwnerPC_G->PlayerState);
		OwnerPC_G->OnPlayerStateUpdateDelegate.AddUObject(this, &USelectRolesBox::OnPlayerStateUpdate);
	}
}

void USelectRolesBox::OnPlayerStateUpdate(APlayerState* PlayerState)
{
	AEveryThingPlayerState_Game* OwnerPlaterState = Cast<AEveryThingPlayerState_Game>(PlayerState);
	if (OwnerPlaterState)
	{
		OnPlayerInfoUpdate(OwnerPlaterState->GetPlayerInfo());
		OwnerPlaterState->OnPlayerInfoUpdateDelegate.AddUObject(this, &USelectRolesBox::OnPlayerInfoUpdate);
	}
}

void USelectRolesBox::OnPlayerInfoUpdate(const FPlayerInfo& InPlayerInfo)
{
	UpdateSelectRolesBoxDisplay(InPlayerInfo.AllHaveRoleNames);
}