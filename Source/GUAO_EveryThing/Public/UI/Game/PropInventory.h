// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PropInventory.generated.h"

class UPropItem;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UPropInventory : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void ClearPropItemList();
	void InitializePropItemList(int32 MaxPropItemNums);

	void OnRoleNameUpdate(const FName& TargetRoleName);
	void OnAllPropsIndexChanged(const TArray<int32>& AllPropsIndex);
	void OnAllPropsChanged();


	UFUNCTION(BlueprintImplementableEvent)
	void AddPropItem(UPropItem* PropItem);
	UFUNCTION(BlueprintImplementableEvent)
	void RemovePropItem(UPropItem* PropItem);
	UFUNCTION(BlueprintImplementableEvent)
	void ClearPropItem();
protected:
	TMap<int32, UPropItem*> PropItemList;

	class UPropComponent* PropComp;
};
