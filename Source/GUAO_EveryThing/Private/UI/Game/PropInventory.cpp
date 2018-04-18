// Fill out your copyright notice in the Description page of Project Settings.

#include "PropInventory.h"

#include "Online/PlayerController_Game.h"
#include "Characters/GamePawn.h"
#include "Characters/PropComponent.h"
#include "UI/Game/PropItem.h"
#include "EveryThingAssetManager.h"

void UPropInventory::NativeConstruct()
{
	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPC_G)
	{
		OnRoleNameUpdate(OwnerPC_G->GetCurrentRoleName());
		OwnerPC_G->OnRoleNameUpdateDelegate.AddUObject(this, &UPropInventory::OnRoleNameUpdate);
	}
}

void UPropInventory::OnRoleNameUpdate(const FName& TargetRoleName)
{
	ClearPropItemList();

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	AGamePawn* OwnerGamePawn = OwnerPC_G ? Cast<AGamePawn>(OwnerPC_G->GetPawn()) : nullptr;
	PropComp = OwnerGamePawn ? OwnerGamePawn->GetPropComponent() : nullptr;
	if (PropComp)
	{
		InitializePropItemList(PropComp->GetMaxPropItemNum());

		OnAllPropsIndexChanged(PropComp->GetAllPropsIndex());
		
		PropComp->OnAllPropsIndexChangedDelegate.AddUObject(this, &UPropInventory::OnAllPropsIndexChanged);
		PropComp->OnAllPropsChangedDelegate.AddUObject(this, &UPropInventory::OnAllPropsChanged);
	}
}

void UPropInventory::InitializePropItemList(int32 MaxPropItemNums)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	TSubclassOf<UUserWidget> PropItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("PropItem");
	if (OwnerPC && PropItemClass)
	{
		for (int32 i = 0; i < MaxPropItemNums; ++i)
		{
			UPropItem* PropItem = CreateWidget<UPropItem>(OwnerPC, PropItemClass);
			if (PropItem)
			{
				PropItem->InitializePropItem(-1);
				AddPropItem(PropItem);

				PropItemList.Add(i, PropItem);
			}
		}
	}
}

void UPropInventory::ClearPropItemList()
{
	PropItemList.Empty();

	ClearPropItem();
}

void UPropInventory::OnAllPropsIndexChanged(const TArray<int32>& AllPropsIndex)
{
	for (int32 i = 0; i < AllPropsIndex.Num(); ++i)
	{
		if (PropItemList.Contains(i))
		{
			PropItemList[i]->InitializePropItem(AllPropsIndex[i]);
		}
	}

	OnAllPropsChanged();
}

void UPropInventory::OnAllPropsChanged()
{
	if (PropComp)
	{
		for (TPair<int32, UPropItem*> PropItem : PropItemList)
		{
			if (PropItem.Value)
			{
				PropItem.Value->UpdatePropNums(PropComp->GetPropNums(PropItem.Value->GetPropID()));
			}
		}
	}
}