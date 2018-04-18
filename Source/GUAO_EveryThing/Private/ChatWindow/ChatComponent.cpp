// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatComponent.h"

#include "Engine/World.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerController.h"


#include "ChatWindow/Channel/SystemChatChannel.h"
#include "ChatWindow/Channel/AllChatChannel.h"
#include "ChatWindow/Channel/TeamChatChannel.h"


#include "ChatWindow/ChatWindowControllerInterface.h"
#include "ChatWindow/ChatWindowGameStateInterface.h"
#include "ChatWindow/ChatWindowPlayerStateInterface.h"
#include "ChatWindow/ChatWindowHUDInterface.h"

UChatComponent::UChatComponent()
{
	FAllChatChannel* DefaultChannel = new FAllChatChannel();
	ChannelManager.AddChatchannelDefaultValue(DefaultChannel, "Default");

	ChannelManager.AddChatchannelDefaultValue(DefaultChannel, "All");
	ChannelManager.AddChatchannelDefaultValue(new FTeamChatChannel(), "Team");
	ChannelManager.AddChatchannelDefaultValue(new FSystemChatChannel(), "System");
}


void UChatComponent::DestroyComponent(bool bPromoteChildren)
{
	ChannelManager.ClearAllChatChannel();

	Super::DestroyComponent(bPromoteChildren);
}

IChatWindowHUDInterface* UChatComponent::GetChatWindowHUD() const
{
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	return OwnerPC ? Cast<IChatWindowHUDInterface>(OwnerPC->GetHUD()) : nullptr;
}

IChatWindowPlayerStateInterface* UChatComponent::GetChatWindowPlayerState() const
{
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	return OwnerPC ? Cast<IChatWindowPlayerStateInterface>(OwnerPC->PlayerState) : nullptr;
}

IChatWindowGameStateInterface* UChatComponent::GetChatWindowGameState() const
{
	UWorld* World = GetOwner() ? GetOwner()->GetWorld() : nullptr;
	return World ? Cast<IChatWindowGameStateInterface>(World->GetGameState()) : nullptr;
}

void UChatComponent::ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	IChatWindowHUDInterface* OwnerCWH = GetChatWindowHUD();
	if (OwnerCWH) { OwnerCWH->ReceiveChatMessage(ChatChannel, ChatMessage); }
}
void UChatComponent::ClientReceiveChatMessage_Implementation(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage)
{
	FChatChannel* ChatChannel = GetChannelManager().GetChatchannelDefaultValueFromName(ChatChannelName);
	if (ChatChannel)
	{
		ReceiveChatMessage(ChatChannel, ChatMessage);
	}
}


void UChatComponent::CreateChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage)
{
	if (ChatChannel) { ServerCreateChatMessage(ChatChannel->GetChannaelName(), ChatMessage); }
}

bool UChatComponent::ServerCreateChatMessage_Validate(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage) { return true; }
void UChatComponent::ServerCreateChatMessage_Implementation(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage)
{
	IChatWindowPlayerStateInterface* OwnerCWPS = GetChatWindowPlayerState();
	IChatWindowGameStateInterface* OwnerCWGS = GetChatWindowGameState();
	FChatChannel* ChatChannel = GetChannelManager().GetChatchannelDefaultValueFromName(ChatChannelName);
	if (OwnerCWPS && OwnerCWGS && ChatChannel)
	{
		for (APlayerState* TargetPS : OwnerCWGS->GetAllChatPlayerState())
		{
			if (ChatChannel->IsNeededToSend(OwnerCWPS, TargetPS))
			{
				IChatWindowControllerInterface* CurrentCWC = Cast<IChatWindowControllerInterface>(TargetPS->GetOwner());
				UChatComponent* CurrentChatComponent = CurrentCWC ? CurrentCWC->GetChatComponent() : nullptr;
				if (CurrentChatComponent)
				{
					CurrentChatComponent->ClientReceiveChatMessage(ChatChannel->GetChannaelName(), ChatMessage);
				}
			}
		}
	}
}