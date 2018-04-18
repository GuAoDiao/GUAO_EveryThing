// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatWindow/Channel/ChatChannel.h"
#include "TeamChatChannel.generated.h"

/**
 * 
 */
USTRUCT()
struct FTeamChatChannel : public FChatChannel
{
	GENERATED_USTRUCT_BODY()
	
	
public:
	FTeamChatChannel();

	virtual bool IsNeededToSend(class IChatWindowPlayerStateInterface* OwnerCWPS, class APlayerState* TargetPS) const;

	virtual FLinearColor GetDisplayLineColor() const override;
	virtual FLinearColor GetDisplayChannelColor() const override;
};
