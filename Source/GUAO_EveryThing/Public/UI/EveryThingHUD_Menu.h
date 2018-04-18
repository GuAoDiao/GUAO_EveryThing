// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "OnlineSessionSettings.h"

#include "EveryThingHUD_Menu.generated.h"


UENUM(BlueprintType)
enum class EMenuUIState : uint8
{
	StartUp,
	MainMenu,

	CreateArchive,
	ArchiveList,

	MasterInterface,
	Storehouse,

	HouseMenu,
	HouseCreate,
	HouseList,

	Config,

	LoadingScreen,
	ErrorDialog,
	Unknown
};

class UUserWidget;

class UMainMenu;

class UCreateArchive;
class UArchiveList;

class UMasterInterface;
class UStorehouse;

class UHouseMenu;
class UHouseList;
class UHouseCreate;

class ULoadingScreen;
class UErrorDialog;


/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingHUD_Menu : public AHUD
{
	GENERATED_BODY()
	
public:
	AEveryThingHUD_Menu();
	virtual void BeginPlay() override;

	//////////////////////////////////////////////////////////////////////////
	/// Game UI State
public:
	UFUNCTION(BlueprintPure)
	EMenuUIState GetCurrentGameUIState() const { return CurrentGameUIState; }
	UFUNCTION(BlueprintPure)
	bool IsTargetGameUIState(EMenuUIState InGameUIState) const { return InGameUIState == CurrentGameUIState; }
	UFUNCTION(BlueprintCallable)
	void ToggleToNewGameUIState(EMenuUIState InGameUIState);
	UFUNCTION(BlueprintCallable)
	void ToggleToLastGameUIState();
private:
	void FinishOldGameUIState(EMenuUIState InGameUIState);
	void StartNewGameUIState(EMenuUIState InGameUIState);
private:
	EMenuUIState CurrentGameUIState;
	EMenuUIState LastGameUIState;

	//////////////////////////////////////////////////////////////////////////
	/// UI
	
public:
	void SetErrorDialogMessage(const FString& ErrorMessage);
	void UpdateHouseList(TArray<FOnlineSessionSearchResult>& SearchResults);
	
private:
	void ShowMainMenu();

	void ShowCreateArchive();
	void ShowArchiveList();

	void ShowMasterInterface();
	void ShowStorehouse();

	void ShowHouseMenu();
	void ShowHouseCreate();
	void ShowHouseList();

	void ShowLoadingScreen();
	void ShowErrorDialog();

	// one help function.
	template<typename T>
	T* CreateAndDisplayWidget(EMenuUIState InNeededUIState, const FName& InUserWidgetName, T* ResultWidget);

	void SetWidgetOwnerAndInputModeToFocusWidget(UUserWidget* InWidget);

protected:
	UPROPERTY(Transient)
	UMainMenu* MainMenu;

	UPROPERTY(Transient)
	UCreateArchive* CreateArchive;
	UPROPERTY(Transient)
	UArchiveList* ArchiveList;

	UPROPERTY(Transient)
	UMasterInterface* MasterInterface;
	UPROPERTY(Transient)
	UStorehouse* Storehouse;

	UPROPERTY(Transient)
	UHouseMenu* HouseMenu;
	UPROPERTY(Transient)
	UHouseList* HouseList;
	UPROPERTY(Transient)
	UHouseCreate* HouseCreate;

	UPROPERTY(Transient)
	ULoadingScreen* LoadingScreen;
	UPROPERTY(Transient)
	UErrorDialog* ErrorDialog;
};
