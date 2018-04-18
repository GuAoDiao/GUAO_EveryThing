// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingHUD_Menu.h"

#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"

#include "UI/Menu/MainMenu.h"

#include "UI/Menu/Archive/CreateArchive.h"
#include "UI/Menu/Archive/ArchiveList.h"

#include "UI/Menu/MasterInterface.h"

#include "UI/Menu/Storehouse/Storehouse.h"

#include "UI/House/HouseMenu.h"
#include "UI/House/HouseList.h"
#include "UI/House/HouseRow.h"
#include "UI/House/HouseCreate.h"

#include "UI/Menu/LoadingScreen.h"
#include "UI/Menu/ErrorDialog.h"


#include "EveryThingGameViewportClient.h"

AEveryThingHUD_Menu::AEveryThingHUD_Menu()
{
	CurrentGameUIState = EMenuUIState::StartUp;
}

void AEveryThingHUD_Menu::BeginPlay()
{
	if (IsTargetGameUIState(EMenuUIState::StartUp))
	{
		UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
		// if has ArchiveName, show MasterInterface
		if (OwnerETGI && !OwnerETGI->GetArchiveName().IsEmpty())
		{
			ToggleToNewGameUIState(EMenuUIState::MasterInterface);
		}
		// else show MainMenu
		else
		{
			ToggleToNewGameUIState(EMenuUIState::MainMenu);
		}
	}

	if (GetOwningPlayerController()) { GetOwningPlayerController()->bShowMouseCursor = true; }
}

//////////////////////////////////////////////////////////////////////////
/// Game UI State

void AEveryThingHUD_Menu::ToggleToNewGameUIState(EMenuUIState InGameUIState)
{
	// if NewUIState isn't LoadingScreen or ErrorDialog, remove old widget 
	if (InGameUIState != EMenuUIState::LoadingScreen && InGameUIState != EMenuUIState::ErrorDialog)
	{
		FinishOldGameUIState(CurrentGameUIState);
	}

	LastGameUIState = CurrentGameUIState;
	CurrentGameUIState = InGameUIState;

	StartNewGameUIState(CurrentGameUIState);
}
void AEveryThingHUD_Menu::ToggleToLastGameUIState()
{
	ToggleToNewGameUIState(LastGameUIState);
}

void AEveryThingHUD_Menu::StartNewGameUIState(EMenuUIState InGameUIState)
{
	switch (InGameUIState)
	{
		case EMenuUIState::StartUp:
			break;
		case EMenuUIState::MainMenu:
			ShowMainMenu();
			break;

		case EMenuUIState::CreateArchive:
			ShowCreateArchive();
			break;
		case EMenuUIState::ArchiveList:
			ShowArchiveList();
			break;

		case EMenuUIState::MasterInterface:
			ShowMasterInterface();
			break;
		case EMenuUIState::Storehouse:
			ShowStorehouse();
			break;

		case EMenuUIState::HouseMenu:
			ShowHouseMenu();
			break;
		case EMenuUIState::HouseCreate:
			ShowHouseCreate();
			break;
		case EMenuUIState::HouseList:
			ShowHouseList();
			break;

		case EMenuUIState::LoadingScreen:
			ShowLoadingScreen();
			break;
		case EMenuUIState::ErrorDialog:
			ShowErrorDialog();
			break;
		case EMenuUIState::Unknown:
			break;
	}
}
void AEveryThingHUD_Menu::FinishOldGameUIState(EMenuUIState InGameUIState)
{
	switch (InGameUIState)
	{
		case EMenuUIState::StartUp:
			break;
		case EMenuUIState::MainMenu:
			if (MainMenu) { MainMenu->RemoveFromParent(); }
			break;

		case EMenuUIState::CreateArchive:
			if (CreateArchive) {CreateArchive->RemoveFromParent();}
			break;
		case EMenuUIState::ArchiveList:
			if (ArchiveList) {ArchiveList->RemoveFromParent();}
			break;

		case EMenuUIState::MasterInterface:
			if (MasterInterface) { MasterInterface->RemoveFromParent(); }
			break;
		case EMenuUIState::Storehouse:
			if (Storehouse) { Storehouse->RemoveFromParent(); }
			break;

		case EMenuUIState::HouseMenu:
			if (HouseMenu) { HouseMenu->RemoveFromParent(); }
			break;
		case EMenuUIState::HouseCreate:
			if (HouseCreate) { HouseCreate->RemoveFromParent(); }
			break;
		case EMenuUIState::HouseList:
			if (HouseList) { HouseList->RemoveFromParent(); }
			break;

		case EMenuUIState::LoadingScreen:
			if (LoadingScreen) { LoadingScreen->RemoveFromParent(); }
			break;
		case EMenuUIState::ErrorDialog:
			if (ErrorDialog) { ErrorDialog->RemoveFromParent(); }
			break;
		case EMenuUIState::Unknown:
			break;
	}
}


//////////////////////////////////////////////////////////////////////////
/// UI

void AEveryThingHUD_Menu::SetErrorDialogMessage(const FString& ErrorMessage)
{
	if (IsTargetGameUIState(EMenuUIState::ErrorDialog) && ErrorDialog)
	{
		ErrorDialog->SetErrorMessage(FText::FromString(ErrorMessage));
	}
}

void AEveryThingHUD_Menu::UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults)
{
	if (IsTargetGameUIState(EMenuUIState::LoadingScreen) && HouseList)
	{
		HouseList->UpdateHouseList(SearchResults);
		ToggleToNewGameUIState(EMenuUIState::HouseList);
	}
}


template<typename T>
T* AEveryThingHUD_Menu::CreateAndDisplayWidget(EMenuUIState InNeededUIState, const FName& InUserWidgetName, T* ResultWidget)
{
	if (IsTargetGameUIState(InNeededUIState))
	{
		// is widget don't exists. try create
		TSubclassOf<UUserWidget> TargetClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(InUserWidgetName);
		if (!ResultWidget && TargetClass)
		{
			ResultWidget = CreateWidget<T>(GetGameInstance(), TargetClass);
		}

		// is widget exists. display and set focus.
		if (ResultWidget)
		{
			if (!ResultWidget->IsInViewport()) { ResultWidget->AddToViewport(); }

			SetWidgetOwnerAndInputModeToFocusWidget(ResultWidget);
		}
	}

	return ResultWidget;
}

//////////////////////////////////////////////////////////////////////////
/// Do Same Thing To Display Widget and Set Focus
void AEveryThingHUD_Menu::ShowMainMenu() { MainMenu = CreateAndDisplayWidget<UMainMenu>(EMenuUIState::MainMenu, "MainMenu", MainMenu); }

void AEveryThingHUD_Menu::ShowCreateArchive() { CreateArchive = CreateAndDisplayWidget<UCreateArchive>(EMenuUIState::CreateArchive, "CreateArchive", CreateArchive); }
void AEveryThingHUD_Menu::ShowArchiveList() { ArchiveList = CreateAndDisplayWidget<UArchiveList>(EMenuUIState::ArchiveList, "ArchiveList", ArchiveList); }

void AEveryThingHUD_Menu::ShowMasterInterface() { MasterInterface = CreateAndDisplayWidget<UMasterInterface>(EMenuUIState::MasterInterface, "MasterInterface", MasterInterface); }
void AEveryThingHUD_Menu::ShowStorehouse() { Storehouse = CreateAndDisplayWidget<UStorehouse>(EMenuUIState::Storehouse, "Storehouse", Storehouse); }

void AEveryThingHUD_Menu::ShowHouseMenu() { HouseMenu = CreateAndDisplayWidget<UHouseMenu>(EMenuUIState::HouseMenu, "HouseMenu", HouseMenu); }
void AEveryThingHUD_Menu::ShowHouseCreate() { HouseCreate = CreateAndDisplayWidget<UHouseCreate>(EMenuUIState::HouseCreate, "HouseCreate", HouseCreate); }
void AEveryThingHUD_Menu::ShowHouseList() { HouseList = CreateAndDisplayWidget<UHouseList>(EMenuUIState::HouseList, "HouseList", HouseList); }

void AEveryThingHUD_Menu::ShowLoadingScreen() { LoadingScreen = CreateAndDisplayWidget<ULoadingScreen>(EMenuUIState::LoadingScreen, "LoadingScreen", LoadingScreen); }
void AEveryThingHUD_Menu::ShowErrorDialog() { ErrorDialog = CreateAndDisplayWidget<UErrorDialog>(EMenuUIState::ErrorDialog, "ErrorDialog", ErrorDialog); }


void AEveryThingHUD_Menu::SetWidgetOwnerAndInputModeToFocusWidget(UUserWidget* InWidget)
{
	APlayerController* OwnerPC = GetGameInstance()->GetFirstLocalPlayerController();
	if (OwnerPC && InWidget)
	{
		InWidget->SetOwningPlayer(OwnerPC);

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(InWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		OwnerPC->SetInputMode(InputMode);
	}
}
