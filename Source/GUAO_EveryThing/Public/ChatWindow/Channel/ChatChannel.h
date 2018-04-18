// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatChannel.generated.h"

/**
 * 
 */
USTRUCT()
struct FChatChannel
{
	GENERATED_USTRUCT_BODY()

public:
	FChatChannel();
	virtual ~FChatChannel() {}

	virtual bool IsSwitchable() const { return true; }

	virtual bool OnToggleToCurrent() const { return true; }
	virtual bool IsNeededToSend(class IChatWindowPlayerStateInterface* OwnerCWPS, class APlayerState* TargetPS) const { return true; }
	virtual FText GetDisplayChannelText() const { return FText::FromString(FString::Printf(*DisplayChannelTextFormat, *ChannelName.ToString())); }
	virtual FText GetDiaplayNameText(const FString& PlayerName) const { return FText::FromString(FString::Printf(*DisplayNameTextFormat, *PlayerName)); }
	virtual const FName& GetChannaelName() const { return ChannelName; }
	virtual FLinearColor GetDisplayLineColor() const { return FLinearColor::Red; }
	virtual FLinearColor GetDisplayChannelColor() const { return FLinearColor::Green; }

protected:
	FString DisplayChannelTextFormat;
	FString DisplayNameTextFormat;
	FName ChannelName;
};
