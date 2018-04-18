// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamChatChannel.h"

#include "GameFramework/PlayerState.h"
#include "ChatWindow/ChatWindowPlayerStateInterface.h"

FTeamChatChannel::FTeamChatChannel()
{
	ChannelName = "Team";
	DisplayNameTextFormat = TEXT("%s Says : ");
}


FLinearColor FTeamChatChannel::GetDisplayLineColor() const { return FLinearColor(1.f, 0.1f, 0.f); }
FLinearColor FTeamChatChannel::GetDisplayChannelColor() const { return FLinearColor(1.f, 0.1f, 0.f); }

bool FTeamChatChannel::IsNeededToSend(class IChatWindowPlayerStateInterface* OwnerCWPS, class APlayerState* TargetPS) const
{
	IChatWindowPlayerStateInterface* TargetCWPS = Cast<IChatWindowPlayerStateInterface>(TargetPS);
	if (OwnerCWPS && TargetPS)
	{
		return OwnerCWPS->GetPlayerChatTeamID() == TargetCWPS->GetPlayerChatTeamID();
	}
	return false;
}