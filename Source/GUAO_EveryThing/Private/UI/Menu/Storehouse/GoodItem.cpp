// Fill out your copyright notice in the Description page of Project Settings.

#include "GoodItem.h"


#include "EveryThingGameInstance.h"


void UGoodsItem::InitializeGoodsItem(int32 InCost, bool bInHaveGoods)
{
	GoodCost = InCost;
	bHaveGoods = bInHaveGoods;
}

bool UGoodsItem::IsHaveEnoughMoney() const
{
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI)
	{
		return OwnerETGI->GetPlayerInfo().Gold >= GoodCost;
	}
	return false;
}