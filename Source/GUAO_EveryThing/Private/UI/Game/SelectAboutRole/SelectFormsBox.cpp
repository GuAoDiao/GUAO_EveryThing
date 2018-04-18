// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectFormsBox.h"

#include "UI/Game/SelectAboutRole/SelectItem.h"
#include "Characters/GamePawn.h"
#include "Characters/GamePawnManager.h"
#include "Online/PlayerController_Game.h"
#include "EveryThingAssetManager.h"

void USelectFormsBox::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPC_G)
	{
		OnRoleNameUpdate(OwnerPC_G->GetCurrentRoleName());
		OwnerPC_G->OnRoleNameUpdateDelegate.AddUObject(this, &USelectFormsBox::OnRoleNameUpdate);
	}
}

void USelectFormsBox::OnRoleNameUpdate(const FName& RoleName)
{
	InitializeSelectFormsBoxDisplay(RoleName);
}

void USelectFormsBox::InitializeSelectFormsBoxDisplay_Implementation(const FName& TargetRoleName)
{
	TSubclassOf<UUserWidget> SelectItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("SelectItem");
	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	AGamePawn* OwnerGamePawn = OwnerPC_G ? Cast<AGamePawn>(OwnerPC_G->GetPawn()) : nullptr;

	if (SelectItemClass && OwnerPC_G && OwnerGamePawn)
	{
		for (const FName& FormName : OwnerGamePawn->AllRoleFormName)
		{
			USelectItem* SelectItem = CreateWidget<USelectItem>(OwnerPC_G, SelectItemClass);
			if (SelectItem)
			{
				SelectItem->InitializeSelectItem(FormName, OwnerGamePawn->AllHaveRoleFormName.Contains(FormName));
				AddSelectItem(SelectItem);
			}

			OwnerGamePawn->OnAllHaveRoleFormNameUpdateDelegate.AddUObject(this, &USelectFormsBox::OnAllHaveRoleFormNameUpdate);
		}

	}
}

void USelectFormsBox::OnAllHaveRoleFormNameUpdate(const TArray<FName>& AllHaveRoleFormName)
{
	UpdateFormItemIsHave(AllHaveRoleFormName);
}