// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Storehouse/GoodItem.h"

#include "EveryThingTypes.h"

#include "RoleItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API URoleItem : public UGoodsItem
{
	GENERATED_BODY()
public:
	void InitializeRoleItem(class UStorehouse* StoreHouse, const FName& InRoleName, bool bInHaveGoods);
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeRoleItemDisplay();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateIsHaveGoods(bool bInHaveGoods);

	UFUNCTION(BlueprintPure)
	const FName& GetRoleName() { return RoleName; }
	UFUNCTION(BlueprintPure)
	const FRoleInfo& GetRoleInfo() { check(RoleInfo); return *RoleInfo; }

	virtual bool BuyGoodsItem() override;

	UFUNCTION(BlueprintCallable)
	void OnBuyRoleItem();
	UFUNCTION(BlueprintCallable)
	void OnDisplayRoleItem();

private:
	const FRoleInfo* RoleInfo;
	FName RoleName;
	UStorehouse* OwnerStoreHouse;
};
