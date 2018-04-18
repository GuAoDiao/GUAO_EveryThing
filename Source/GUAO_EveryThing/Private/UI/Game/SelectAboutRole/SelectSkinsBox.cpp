// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectSkinsBox.h"

#include "Online/EveryThingPlayerState_Game.h"
#include "Online/PlayerController_Game.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"
#include "SelectItem.h"
#include "Characters/GamePawn.h"

void USelectSkinsBox::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPC_G)
	{
		OnRoleNameUpdate(OwnerPC_G->GetCurrentRoleName());
		OwnerPC_G->OnRoleNameUpdateDelegate.AddUObject(this, &USelectSkinsBox::OnRoleNameUpdate);
	}
}

void USelectSkinsBox::OnRoleNameUpdate(const FName& RoleName)
{
	InitializeSelectSkinsBoxDisplay(RoleName);
}

void USelectSkinsBox::InitializeSelectSkinsBoxDisplay_Implementation(const FName& TargetRoleName)
{
	TSubclassOf<UUserWidget> SelectItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("SelectItem");
	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	AGamePawn* OwnerGamePawn = OwnerPC_G ? Cast<AGamePawn>(OwnerPC_G->GetPawn()) : nullptr;

	if (SelectItemClass && OwnerPC_G && OwnerGamePawn)
	{
		for (const FName& SkinName : OwnerGamePawn->AllRoleSkinName)
		{
			USelectItem* SelectItem = CreateWidget<USelectItem>(OwnerPC_G, SelectItemClass);
			if (SelectItem)
			{
				SelectItem->InitializeSelectItem(SkinName, OwnerGamePawn->AllHaveRoleSkinName.Contains(SkinName));
				AddSelectItem(SelectItem);
			}
		}

		OwnerGamePawn->OnAllHaveRoleSkinNameUpdateDelegate.AddUObject(this, &USelectSkinsBox::OnAllHaveRoleSkinNameUpdate);
	}

}

void USelectSkinsBox::OnAllHaveRoleSkinNameUpdate(const TArray<FName>& AllHaveSkinFormName)
{
	UpdateSkinItemIsHave(AllHaveSkinFormName);
}