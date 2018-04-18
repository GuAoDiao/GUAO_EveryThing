// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenu.h"

#include "Engine/World.h"
#include "GameFramework/InputSettings.h"


#include "EveryThingGameInstance.h"
#include "UI/EveryThingHUD_Game.h"

UGameMenu::UGameMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UGameMenu::NativeConstruct()
{
	UInputSettings* InpueSettings = UInputSettings::GetInputSettings();
	if (InpueSettings)
	{
		TArray<FInputActionKeyMapping> EscapeActionMapping;
		InpueSettings->GetActionMappingByName("ToggleGameMenu", EscapeActionMapping);
		for (FInputActionKeyMapping& EscapeAction : EscapeActionMapping)
		{
			EscapeKeys.Add(EscapeAction.Key);
		}
	}

	Super::NativeConstruct();
}

FReply UGameMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (EscapeKeys.Contains(InKeyEvent.GetKey()))
	{
		Backup();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UGameMenu::Confing()
{

}

void UGameMenu::Backup()
{
	AEveryThingHUD_Game* OwnerETGH = GetOwningPlayer() ? Cast<AEveryThingHUD_Game>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (OwnerETGH) {OwnerETGH->RemoveGameMenu();}
}

void UGameMenu::ExitGame()
{
	UEveryThingGameInstance* OwnerGameInstance = GetWorld() ? GetWorld()->GetGameInstance<UEveryThingGameInstance>() : nullptr;
	if (OwnerGameInstance) { OwnerGameInstance->ExitGame(); }
}