// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ChatWindow/Channel/ChatChannel.h"
#include "ChatWindow/ChatWindowTypes.h"
#include "ChatWindow/Channel/ChannelManager.h"

#include "ChatComponent.generated.h"

class IChatWindowHUDInterface;
class IChatWindowPlayerStateInterface;
class IChatWindowGameStateInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChatComponent();

	virtual void DestroyComponent(bool bPromoteChildren) override;
	
	IChatWindowHUDInterface* GetChatWindowHUD() const;
	IChatWindowPlayerStateInterface* GetChatWindowPlayerState() const;
	IChatWindowGameStateInterface* GetChatWindowGameState() const;

	FChannelManager& GetChannelManager() { return ChannelManager; }

public:
	void ReceiveChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage);
	UFUNCTION(client, Reliable)
	void ClientReceiveChatMessage(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage);

	void CreateChatMessage(const FChatChannel* ChatChannel, const FChatMessageInfo& ChatMessage);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCreateChatMessage(const FName& ChatChannelName, const FChatMessageInfo& ChatMessage);

	FChannelManager ChannelManager;
};
