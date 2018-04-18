// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatWindow/Channel/ChatChannel.h"

#include "ChatWindow/ChatWindowTypes.h"

#include "SystemChatChannel.generated.h"

/**
 * 
 */
USTRUCT()
struct FSystemChatChannel : public FChatChannel
{
	GENERATED_USTRUCT_BODY()
	
public:
	FSystemChatChannel();	
	
	virtual FLinearColor GetDisplayLineColor() const override;

	virtual bool IsSwitchable() const { return false; }

	virtual FText GetDiaplayNameText(const FString& PlayerName) const { return FText::GetEmpty(); }

public:

	ESystemMessageType GetMessageType() const { return MessageType; }
	void SetMessageType(ESystemMessageType InMessageType) { MessageType = InMessageType; }
	void SetDisplayColor(FLinearColor InDisplayColor) { DisplayColor = InDisplayColor; }
protected:
	UPROPERTY()
	FLinearColor DisplayColor;

	UPROPERTY()
	ESystemMessageType MessageType;
};
