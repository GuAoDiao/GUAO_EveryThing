// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Storehouse/GoodItem.h"

#include "EveryThingTypes.h"

#include "SkinItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API USkinItem : public UGoodsItem
{
	GENERATED_BODY()
	
public:
	void InitializeSkinItem(class UStorehouse* StoreHouse, const FName& InSkinName, bool bInHaveGoods);
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeSkinItemDisplay();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateIsHaveGoods(bool bInHaveGoods);


	UFUNCTION(BlueprintPure)
	const FName& GetSkinName() { return SkinName; }
	UFUNCTION(BlueprintPure)
	const FRoleSkinInfo& GetSkinInfo() {check(SkinInfo); return *SkinInfo;}

	UFUNCTION(BlueprintCallable)
	void OnDisplaySkinItem();

	UFUNCTION(BlueprintCallable)
	void OnBuySkinItem();
	
	virtual bool BuyGoodsItem() override;
private:
	const FRoleSkinInfo* SkinInfo;
	FName SkinName;
	UStorehouse* OwnerStoreHouse;
};
