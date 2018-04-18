// Fill out your copyright notice in the Description page of Project Settings.

#include "FormItem.h"


#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "EveryThingGameInstance.h"


void UFormItem::InitializeFormItem(const FName& InFormName, bool bInHaveGoods)
{
	UGamePawnManager* GamePawnManager = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager();
	if (GamePawnManager->GetRoleFormInfo(InFormName, FormInfo) && FormInfo)
	{
		InitializeGoodsItem(FormInfo->Cost, bInHaveGoods);

		InitializeFormItemDisplay();

		UpdateIsHaveGoods(bInHaveGoods);
	}
}

void UFormItem::OnBuyFormItem() { BuyGoodsItem(); }


bool UFormItem::BuyGoodsItem()
{
	if (FormInfo && IsHaveEnoughMoney())
	{
		UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
		if (OwnerETGI)
		{
			FPlayerInfo& PlayerInfo = OwnerETGI->GetPlayerInfo();
			PlayerInfo.AllHaveRoleFormNames.AddUnique(FormInfo->Name);
			PlayerInfo.Gold -= GoodCost;

			OwnerETGI->UpdatePlayerInfo();

			UpdateIsHaveGoods(true);

			return true;
		}
	}
	return false;
}



