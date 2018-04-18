// Fill out your copyright notice in the Description page of Project Settings.

#include "ChannelManager.h"

#include "ChatWindow/Channel/ChatChannel.h"


void FChannelManager::AddChatchannelDefaultValue(FChatChannel* ChatChanelClass, const FName& ChatChannelName)
{
	AllChatChannels.Add(ChatChannelName, ChatChanelClass);
}

FChatChannel* FChannelManager::GetChatchannelDefaultValueFromName(const FName& ChatChannelName) const
{
	if (AllChatChannels.Contains(ChatChannelName))
	{
		return AllChatChannels[ChatChannelName];
	}
	return nullptr;
}

void FChannelManager::ClearAllChatChannel()
{
	for (TMap<FName, FChatChannel*>::TIterator It(AllChatChannels); It; ++It)
	{
		if (It.Value()) { delete It.Value(); }
	}
	AllChatChannels.Empty();
}