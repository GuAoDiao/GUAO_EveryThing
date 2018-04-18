// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatInput.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

#include "ChatWindow/UI/ChatWindow.h"
#include "ChatWindow/Channel/ChannelManager.h"
#include "ChatWindow/ChatComponent.h"
#include "ChatWindow/Channel/ChatChannel.h"
#include "ChatWindow/ChatWindowHUDInterface.h"
#include "ChatWindow/ChatWindowPlayerStateInterface.h"

void UChatInput::InitializeChatInput_Implementation(UChatWindow* InChatWindow)
{
	ChatWindow = InChatWindow;
}

void UChatInput::ProcessInput(const FText& InputText)
{
	FString InputString = InputText.ToString();

	FString Shortcut, Message;
	if (InputString.Split(TEXT(" "), &Shortcut, &Message, ESearchCase::IgnoreCase, ESearchDir::FromStart))
	{
		UChatComponent* ChatComponent = ChatWindow ? ChatWindow->GetChatComponent() : nullptr;
		FName ShorcutName = FName(*Shortcut);
		if (ChatComponent && ChatComponent->GetChannelManager().IsExistChannelName(ShorcutName))
		{
			FChatChannel* ChatChannel = ChatComponent->GetChannelManager().GetChatchannelDefaultValueFromName(ShorcutName);
			if (ChatWindow->ToggleChatChannel(ChatChannel))
			{
				// remove first /
				SetInputText(FText::FromString(Message.StartsWith(TEXT("/")) ? Message.Mid(1) : Message));
			}
		}
	}
}

bool UChatInput::IsCommand(const FText& Command) const
{
	FString CommandString = Command.ToString();

	return CommandString.StartsWith(TEXT("/"), ESearchCase::IgnoreCase) && CommandString.Len() > 1;
}

void UChatInput::ExcuteInput(const FText& InputText)
{
	// if empty remove chat focus
	if (InputText.IsEmpty())
	{
		IChatWindowHUDInterface* OwnerCWH = Cast<IChatWindowHUDInterface>(GetOwningPlayer()->GetHUD());
		if (OwnerCWH) { OwnerCWH->RemoveChatWidnowFocus(); }
		return;
	}

	
	UChatComponent* ChatComponent = ChatWindow ? ChatWindow->GetChatComponent() : nullptr;
	IChatWindowPlayerStateInterface* OwnerCWPS = GetOwningPlayerPawn() ? Cast<IChatWindowPlayerStateInterface>(GetOwningPlayerPawn()->PlayerState) : nullptr;
	if (ChatWindow && ChatComponent && OwnerCWPS)
	{
		FString InputString = InputText.ToString();

		// send message or execute command
		if (InputString.StartsWith(TEXT("/"), ESearchCase::IgnoreCase))
		{
			// ChatComponent->ClientExecuteCommandEvent(InputString);
		}
		else
		{	
			FChatChannel* CurrentChatChannel = ChatWindow->GetChatChannel();
			if (CurrentChatChannel)
			{
				FChatMessageInfo MessageInfo(InputString, CurrentChatChannel->GetDisplayLineColor(), OwnerCWPS->GetPlayerChatID());
				ChatComponent->CreateChatMessage(ChatWindow->GetChatChannel(), MessageInfo);
			}
		}
	}

	SetInputText(FText());

	FocusToInputText();
}