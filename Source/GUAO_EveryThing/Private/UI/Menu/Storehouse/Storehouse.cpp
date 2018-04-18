// Fill out your copyright notice in the Description page of Project Settings.

#include "Storehouse.h"

#include "EngineUtils.h"
#include "GameFramework/InputSettings.h"

#include "EveryThingAssetManager.h"
#include "EveryThingGameInstance.h"
#include "Characters/GamePawnManager.h"
#include "UI/EveryThingHUD_Menu.h"
#include "UI/Role3DDisplay.h"
#include "UI/Menu/Storehouse/RoleItem.h"
#include "UI/Menu/Storehouse/SkinItem.h"
#include "UI/Menu/Storehouse/FormItem.h"
#include "Characters/Form/GamePawnFormClassInfo.h"
#include "Characters/Skin/GamePawnSkinClassInfo.h"


UStorehouse::UStorehouse(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
	CurrentDisplayRoleName = "Football";
}

void UStorehouse::NativeConstruct()
{
	InitializeRoleListDisplay();
	ToggleDisplayRole(CurrentDisplayRoleName);

	// Initialize Display with player info and bind delegate.
	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI)
	{
		OnPlayerInfoUpdate(OwnerETGI->GetPlayerInfo());
		OwnerETGI->OnPlayerInfoUpdateDelegate.AddUObject(this, &UStorehouse::OnPlayerInfoUpdate);
	}
	
	// try get first Role 3D Display
	UWorld* World = GetWorld();
	if (World)
	{
		TActorIterator<ARole3DDisplay> It(World);
		if (It) { Role3DDisplay = *It; }
	}

	// get MoveForward and MoveRight binding key.
	UInputSettings* InpueSettings = UInputSettings::GetInputSettings();
	if (InpueSettings)
	{
		TArray<FInputAxisKeyMapping> AxisMapping;
		InpueSettings->GetAxisMappingByName("MoveForward", AxisMapping);
		for (FInputAxisKeyMapping& AdjustUPAxis : AxisMapping)
		{
			AdjustUPKey.Add(AdjustUPAxis.Key, AdjustUPAxis.Scale);
		}

		InpueSettings->GetAxisMappingByName("MoveRight", AxisMapping);
		for (FInputAxisKeyMapping& AdjustRightAxis : AxisMapping)
		{
			AdjustRightKey.Add(AdjustRightAxis.Key, AdjustRightAxis.Scale);
		}
	}


	Super::NativeConstruct();
}





void UStorehouse::InitializeRoleListDisplay_Implementation()
{
	TSubclassOf<UUserWidget> RoleItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("RoleItem");
	APlayerController* OnwerPC = GetOwningPlayer();

	if (RoleItemClass && OnwerPC)
	{
		const TMap<FName, FRoleInfo>& AllRolesInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetAllRolesInfo();
		for (TMap<FName, FRoleInfo>::TConstIterator It(AllRolesInfo); It; ++It)
		{
			URoleItem* RoleItem = CreateWidget<URoleItem>(OnwerPC, RoleItemClass);
			if (RoleItem)
			{
				RoleItem->InitializeRoleItem(this, It.Key(), false);
				AddRoleItem(RoleItem);
			}
		}
	}
}

void UStorehouse::InitializeRoleSkinAndFormListDisplay_Implementation(const FPlayerInfo& InPlayerInfo)
{
	APlayerController* OnwerPC = GetOwningPlayer();

	UEveryThingAssetManager* AssetManager = UEveryThingAssetManager::GetAssetManagerInstance();
	UGamePawnManager* GamePawnManager= AssetManager->GetGamePawnManager();

	// create skin and form item.

	TSubclassOf<UUserWidget> SkinItemClass = AssetManager->GetUserWidgetFromName("SkinItem");
	if (SkinItemClass)
	{
		const TArray<FName>& AllRoleSkinName = GamePawnManager->GetAllRoleSkinWithRoleName(CurrentDisplayRoleName);
		for (const FName& SkinName : AllRoleSkinName)
		{
			USkinItem* SkinItem = CreateWidget<USkinItem>(OnwerPC, SkinItemClass);
			if (SkinItem)
			{
				SkinItem->InitializeSkinItem(this, SkinName, InPlayerInfo.AllHaveRoleSkinNames.Contains(SkinName));
				AddSkinItem(SkinItem);
			}
		}
	}


	TSubclassOf<UUserWidget> FormItemClass = AssetManager->GetUserWidgetFromName("FormItem");
	if (FormItemClass)
	{
		const TArray<FName>& AllRoleFormName = GamePawnManager->GetAllRoleFormWithRoleName(CurrentDisplayRoleName);
		for (const FName& FormName : AllRoleFormName)
		{
			UFormItem* FormItem = CreateWidget<UFormItem>(OnwerPC, FormItemClass);
			if (FormItem)
			{
				FormItem->InitializeFormItem(FormName, InPlayerInfo.AllHaveRoleFormNames.Contains(FormName));
				AddFormItem(FormItem);
			}
		}
	}
}

void UStorehouse::Backup()
{
	AEveryThingHUD_Menu* OwnerMenuHUD = GetOwningPlayer() ? Cast<AEveryThingHUD_Menu>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerMenuHUD) { OwnerMenuHUD->ToggleToNewGameUIState(EMenuUIState::MasterInterface); }
}


//////////////////////////////////////////////////////////////////////////
/// Role 3d Display

FReply UStorehouse::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey& Key = InKeyEvent.GetKey();

	// when key down, adjust display role.
	if (AdjustUPKey.Contains(Key)) { AdjustDisplayRoleUp(AdjustUPKey[Key]); }
	if (AdjustRightKey.Contains(Key)) { AdjustDisplayRoleRight(AdjustRightKey[Key]); }

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);

}

void UStorehouse::AdjustDisplayRoleUp(float AxisValue) { if (Role3DDisplay) { Role3DDisplay->AdjustUp(AxisValue); } }
void UStorehouse::AdjustDisplayRoleRight(float AxisValue) { if (Role3DDisplay) { Role3DDisplay->AdjustRight(AxisValue); } }

void UStorehouse::ToggleDisplayRole(const FName& RoleName)
{
	CurrentDisplayRoleName = RoleName;

	UEveryThingGameInstance* OwnerETGI = GetOwningPlayer() ? Cast<UEveryThingGameInstance>(GetOwningPlayer()->GetGameInstance()) : nullptr;
	if (OwnerETGI) { InitializeRoleSkinAndFormListDisplay(OwnerETGI->GetPlayerInfo()); }

	if (Role3DDisplay) { Role3DDisplay->ChangeRole(RoleName); }
}
void UStorehouse::ToggleDisplaySkin(const FName& SkinName) { if (Role3DDisplay) { Role3DDisplay->ChangeSkin(SkinName); } }