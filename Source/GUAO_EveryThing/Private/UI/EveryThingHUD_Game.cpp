// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingHUD_Game.h"

#include "UI/Game/GameMenu.h"
#include "UI/Game/GameLayout.h"
#include "UI/Game/GameFlowPath/GameOverDisplay.h"
#include "UI/Game/GameInfo/ScoreBox.h"
#include "EveryThingAssetManager.h"

#include "ChatWindow/UI/ChatWindow.h"
#include "ChatWIndow/UI/ChatLine.h"

#include "Online/EveryThingGameState_Game.h"

AEveryThingHUD_Game::AEveryThingHUD_Game()
{
	CurrentGameUIState = EETGameState::WaitForHousePlayerLoad;
}

void AEveryThingHUD_Game::BeginPlay()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// remove all widget, because if from ServerTravel, the HouseLayout will display in screen.
		UGameViewportClient* OwnerGameViewport =  World->GetGameViewport();
		if (OwnerGameViewport) { OwnerGameViewport->RemoveAllViewportWidgets(); }

		// update current UI state
		AEveryThingGameState_Game* OwnerETGS_G = World->GetGameState<AEveryThingGameState_Game>();
		if (OwnerETGS_G) { CurrentGameUIState = OwnerETGS_G->GetETGameState(); }
	}

	// try create GameLatyout and initialize then display
	APlayerController* OwnerPC = GetOwningPlayerController();
	TSubclassOf<UUserWidget> GameLayoutClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("GameLayout"));
	if (OwnerPC && GameLayoutClass)
	{
		GameLayout = CreateWidget<UGameLayout>(OwnerPC, GameLayoutClass);
		
		if (GameLayout)
		{
			GameLayout->InitializeGameLayout();
			ChatWindow = GameLayout->ChatWindow;
			InitializeChatWindow();
			GameLayout->UpdateGameUIDisplay(CurrentGameUIState);

			GameLayout->AddToViewport();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Game UI State

void AEveryThingHUD_Game::ToggleToTargetGameUIState(EETGameState InGameUIState)
{
	CurrentGameUIState = InGameUIState;

	if (GameLayout) { GameLayout->UpdateGameUIDisplay(CurrentGameUIState); }
}

void AEveryThingHUD_Game::ShowGameOver(int32 GetGold)
{
	if (!GameOver)
	{
		TSubclassOf<UUserWidget> GameOverClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("GameOverDisplay"));
		if (GameOverClass)
		{
			GameOver = CreateWidget<UGameOverDisplay>(GetOwningPlayerController(), GameOverClass);
			GameOver->InitializeGameOverDisplay(GetGold);
		}
	}

	if (GameOver)
	{
		GameOver->AddToViewport();

		APlayerController* OwnerPC = GetOwningPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(GameOver->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			OwnerPC->SetInputMode(InputMode);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Game Menu
void AEveryThingHUD_Game::DisplayGameMenu()
{
	if (!GameMenu)
	{
		TSubclassOf<UUserWidget> GameMenuClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("GameMenu"));
		if (GameMenuClass)
		{
			GameMenu = CreateWidget<UGameMenu>(GetOwningPlayerController(), GameMenuClass);
		}
	}

	if (GameMenu)
	{
		GameMenu->AddToViewport();

		APlayerController* OwnerPC = GetOwningPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(GameMenu->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			OwnerPC->SetInputMode(InputMode);
		}
	}
}
void AEveryThingHUD_Game::RemoveGameMenu()
{
	if (GameMenu && GameMenu->IsInViewport()) { GameMenu->RemoveFromViewport(); }

	APlayerController* OwnerPC = GetOwningPlayerController();
	if (OwnerPC)
	{
		OwnerPC->bShowMouseCursor = false;
		OwnerPC->SetInputMode(FInputModeGameOnly());
	}
}


//////////////////////////////////////////////////////////////////////////
/// Score Box
void AEveryThingHUD_Game::DisplayScoreBoard()
{
	if (!ScoreBoard)
	{
		TSubclassOf<UUserWidget> ScoreBoardClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName(TEXT("ScoreBoard"));
		if (ScoreBoardClass)
		{
			ScoreBoard = CreateWidget<UScoreBoard>(GetOwningPlayerController(), ScoreBoardClass);
		}
	}

	if (ScoreBoard && !ScoreBoard->IsInViewport()) { ScoreBoard->AddToViewport(); }
}

void AEveryThingHUD_Game::RemoveScoreBoard()
{
	if (ScoreBoard && ScoreBoard->IsInViewport()) { ScoreBoard->RemoveFromViewport(); }

	APlayerController* OwnerPC = GetOwningPlayerController();
	if (OwnerPC)
	{
		OwnerPC->SetInputMode(FInputModeGameOnly());
	}
}

//////////////////////////////////////////////////////////////////////////
/// Player Fight Info
void AEveryThingHUD_Game::ClientCreatePlayerFightInfo(const FText& FightInfo) { if (GameLayout) { GameLayout->ClientCreatePlayerFightInfo(FightInfo); } }

//////////////////////////////////////////////////////////////////////////
/// Select About Roles 

void AEveryThingHUD_Game::ToggleSelectRolesBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectRolesBox(); }
		else { GameLayout->RemoveSelectRolesBox(); }
	}
}

void AEveryThingHUD_Game::ToggleSelectSkinsBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectSkinsBox(); }
		else { GameLayout->RemoveSelectSkinsBox(); }
	}
}

void AEveryThingHUD_Game::ToggleSelectFormsBox(bool bIsDisplay)
{
	if (GameLayout)
	{
		if (bIsDisplay) { GameLayout->DisplaySelectFormsBox(); }
		else { GameLayout->RemoveSelectFormsBox(); }
	}
}


//////////////////////////////////////////////////////////////////////////
/// For Chat Window HUD Interface

void AEveryThingHUD_Game::InitializeChatWindow()
{
	if (ChatWindow) { ChatWindow->InitializeChatWindow(); }
}

void AEveryThingHUD_Game::FocusToChatWindow()
{
	if (ChatWindow)
	{
		APlayerController* OwnerPC = GetOwningPlayerController();
		if (OwnerPC)
		{
			OwnerPC->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(ChatWindow->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			OwnerPC->SetInputMode(InputMode);
		}

		FocusToChatInput();
	}
}

void AEveryThingHUD_Game::RemoveChatWidnowFocus()
{
	APlayerController* OwnerPC = GetOwningPlayerController();
	if (OwnerPC)
	{
		OwnerPC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		OwnerPC->SetInputMode(InputMode);
	}
}

void AEveryThingHUD_Game::FocusToChatInput() { if (ChatWindow) { ChatWindow->FocusToChatInput(); } }
void AEveryThingHUD_Game::FocusToChatReply() { if (ChatWindow) { ChatWindow->FocusToChatReply(); } }
void AEveryThingHUD_Game::FocusToChatCommand() { if (ChatWindow) { ChatWindow->FocusToChatCommand(); } }

TSubclassOf<UUserWidget> AEveryThingHUD_Game::GetChatLineWidgetClass()
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ChatLine");
}
TSubclassOf<UUserWidget> AEveryThingHUD_Game::GetChatWindowWidgetClass()
{
	return UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ChatWindow");
}

void AEveryThingHUD_Game::ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	if (ChatWindow) { ChatWindow->ReceiveChatMessage(ChatChannel, ChatMessage); }
}
