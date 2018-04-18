// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ChatWindow/Channel/ChatChannel.h"

#include "ChannelManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FChannelManager
{
	GENERATED_USTRUCT_BODY()
public:
	void AddChatchannelDefaultValue(FChatChannel* ChatChanel, const FName& ChatChannelName = NAME_None);

	FChatChannel* GetChatchannelDefaultValueFromName(const FName& ChatChannelName) const;

	FChatChannel* GetDefaultChannel() const { return GetChatchannelDefaultValueFromName("Default"); }

	bool IsExistChannelName(const FName& ChatChannelName) const { return AllChatChannels.Contains(ChatChannelName); }

	void ClearAllChatChannel();
private:
	TMap<FName, FChatChannel*> AllChatChannels;
};
