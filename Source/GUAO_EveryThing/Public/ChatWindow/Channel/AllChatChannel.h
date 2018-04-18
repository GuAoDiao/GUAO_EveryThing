// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatWindow/Channel/ChatChannel.h"
#include "AllChatChannel.generated.h"

/**
 * 
 */
USTRUCT()
struct FAllChatChannel : public FChatChannel
{
	GENERATED_USTRUCT_BODY()
	
public:
	FAllChatChannel();
	
	virtual FLinearColor GetDisplayLineColor() const override;
	virtual FLinearColor GetDisplayChannelColor() const override;

};
