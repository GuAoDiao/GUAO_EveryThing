// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Storehouse/GoodItem.h"

#include "EveryThingTypes.h"

#include "FormItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UFormItem : public UGoodsItem
{
	GENERATED_BODY()
	
public:
	void InitializeFormItem(const FName& InFormName, bool bInHaveGoods);
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeFormItemDisplay();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateIsHaveGoods(bool bInHaveGoods);

	UFUNCTION(BlueprintPure)
	const FRoleFormInfo& GetFormInfo() const { check(FormInfo); return *FormInfo; }

	UFUNCTION(BlueprintCallable)
	void OnBuyFormItem();
	
	virtual bool BuyGoodsItem() override;
private:
	const FRoleFormInfo* FormInfo;
};
