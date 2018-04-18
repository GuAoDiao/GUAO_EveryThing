// Fill out your copyright notice in the Description page of Project Settings.

#include "RoleItem.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "UI/Menu/Storehouse/Storehouse.h"


void URoleItem::InitializeRoleItem(UStorehouse* StoreHouse, const FName& InRoleName, bool bInHaveGoods)
{
	RoleName = InRoleName;
	OwnerStoreHouse = StoreHouse;

	UGamePawnManager* GamePawnManager = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager();
	if (GamePawnManager->GetRoleInfoFromName(InRoleName, RoleInfo) && RoleInfo)
	{
		InitializeGoodsItem(RoleInfo->Cost, bInHaveGoods);

		InitializeRoleItemDisplay();

		UpdateIsHaveGoods(bInHaveGoods);
	}
}

void URoleItem::OnBuyRoleItem() { BuyGoodsItem(); }
bool URoleItem::BuyGoodsItem()
{
	if (IsHaveEnoughMoney())
	{
		UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
			PlayerInfo.AllHaveRoleNames.AddUnique(RoleName);
			
			if (RoleInfo)
			{
				PlayerInfo.AllHaveRoleSkinNames.AddUnique(RoleInfo->DefaultSkinName);
				PlayerInfo.AllHaveRoleFormNames.AddUnique(RoleInfo->DefaultFormName);
			}

			PlayerInfo.Gold -= GoodCost;
			
			OwnerETGI->UpdatePlayerInfo();

			UpdateIsHaveGoods(true);

			return true;
		}
	}
	return false;
}

void URoleItem::OnDisplayRoleItem() { if (OwnerStoreHouse) { OwnerStoreHouse->ToggleDisplayRole(RoleName); } }
