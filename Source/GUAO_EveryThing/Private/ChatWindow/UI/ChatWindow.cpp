// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatWindow.h"

#include "GameFramework/HUD.h"
#include "GameFramework/GameStateBase.h"
#include "Package.h"

#include "ChatWindow/UI/ChatInput.h"
#include "ChatWindow/UI/ChatLine.h"
#include "ChatWindow/ChatWindowControllerInterface.h"
#include "ChatWindow/ChatWindowHUDInterface.h"
#include "ChatWindow/ChatWindowGameStateInterface.h"
#include "ChatWindow/ChatComponent.h"
#include "ChatWindow/Channel/ChannelManager.h"
#include "ChatWindow/Channel/ChatChannel.h"

void UChatWindow::NativeConstruct()
{
	InitializeChatWindow();
	
	if (ChatInput) { ChatInput->InitializeChatInput(this); }
	
	Super::NativeConstruct();
}

void UChatWindow::InitializeChatWindow_Implementation()
{
	IChatWindowControllerInterface* OwnerCWC = Cast<IChatWindowControllerInterface>(GetOwningPlayer());
	if (OwnerCWC)
	{
		ChatComponent = OwnerCWC->GetChatComponent();
		ToggleChatChannel(ChatComponent->GetChannelManager().GetDefaultChannel());
	}
}


/// Chat input
void UChatWindow::FocusToChatInput() { if (ChatInput) { ChatInput->FocusToInputText(); } }

void UChatWindow::FocusToChatCommand()
{
	if (ChatInput)
	{
		ChatInput->SetInputText(FText::FromString(TEXT("/")));
		ChatInput->FocusToInputText();
	}
}

void UChatWindow::FocusToChatReply()
{
	if (ChatInput)
	{
		ChatInput->SetInputText(FText::FromString(TEXT("/r")));
		ChatInput->FocusToInputText();
	}
}


/// Chat Channel 
bool UChatWindow::ToggleChatChannel(FChatChannel* InChatChannel)
{
	
	if (InChatChannel && InChatChannel->IsSwitchable())
	{
		ChatChannel = InChatChannel;
		ChatChannel->OnToggleToCurrent();
		UpdateChannelText(ChatChannel->GetDisplayChannelText(), ChatChannel->GetDisplayChannelColor());
		return true;
	}
	return false;
}

void UChatWindow::ToggleToReplayWithPlayerID(int32 PlayerID)
{

}



/// Chat Line
void UChatWindow::ReceiveChatMessage(const FChatChannel* InChatChannel, const FChatMessageInfo& ChatMessage)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	if (!OwnerPC) { return; }

	IChatWindowHUDInterface* OwnerCWH = Cast<IChatWindowHUDInterface>(OwnerPC->GetHUD());
	IChatWindowGameStateInterface* OwnerCWGS = OwnerPC->GetWorld() ? Cast<IChatWindowGameStateInterface>(OwnerPC->GetWorld()->GetGameState()) : nullptr;

	if (OwnerCWH && OwnerCWGS && InChatChannel)
	{
		TSubclassOf<UUserWidget> ChatLineClass = OwnerCWH->GetChatLineWidgetClass();
		UChatLine* NewChatLine = ChatLineClass ? CreateWidget<UChatLine>(GetOwningPlayer(), ChatLineClass) : nullptr;
		if (NewChatLine)
		{
			NewChatLine->InitalizeChatLine(this, 
				InChatChannel->GetChannaelName() , InChatChannel->GetDisplayChannelText(), 
				ChatMessage.PlayerID, InChatChannel->GetDiaplayNameText(OwnerCWGS->GetPlayerChatName(ChatMessage.PlayerID)),
				FText::FromString(ChatMessage.Message), ChatMessage.DisplayColor);

			AddChatLine(NewChatLine);
		}
	}
}