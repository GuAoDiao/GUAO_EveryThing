// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoodItem.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GUAO_EVERYTHING_API UGoodsItem : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	void InitializeGoodsItem(int32 InCost, bool bInHaveGoods);
	
	bool IsHaveEnoughMoney() const;

	virtual bool BuyGoodsItem() { return false; }
protected:
	int32 GoodCost;
	bool bHaveGoods;
};
