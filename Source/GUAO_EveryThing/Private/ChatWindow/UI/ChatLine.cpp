// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatLine.h"

#include "Kismet/KismetMathLibrary.h"

#include "ChatWindow/ChatComponent.h"
#include "ChatWindow/UI/ChatWindow.h"


void UChatLine::InitalizeChatLine_Implementation(UChatWindow* InChatWindow, const FName& InChatChannelName, const FText& InChanenelText, int32 InPlayerID, const FText& InNameText, const FText& InMessageText, const FLinearColor& InDisplayColor)
{
	ChatWindow = InChatWindow;
	ChatChannelName = InChatChannelName;
	PlayerID = InPlayerID;

	FDateTime Time = UKismetMathLibrary::Now();
	UpdateTimeStamp(FText::FromString(FString::Printf(TEXT("<%02d:%02d:%02d> "), Time.GetHour(), Time.GetMinute(), Time.GetSecond())));
}

void UChatLine::OnClickedName()
{
	if (ChatWindow) { ChatWindow->ToggleToReplayWithPlayerID(PlayerID); }
}

void UChatLine::OnClickedChannel()
{	
	if (ChatWindow && ChatWindow->GetChatComponent())
	{
		ChatWindow->ToggleChatChannel(ChatWindow->GetChatComponent()->GetChannelManager().GetChatchannelDefaultValueFromName(ChatChannelName));
	}
}
