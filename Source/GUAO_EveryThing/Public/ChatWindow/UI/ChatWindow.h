// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ChatWindowTypes.h"
#include "ChatWindow/Channel/ChatChannel.h"

#include "ChatWindow.generated.h"

class UChatComponent;
class UChatInput;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UChatWindow : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeChatWindow();

	//////////////////////////////////////////////////////////////////////////
	/// Chat Component
public:
	UChatComponent* GetChatComponent() const { return ChatComponent; }
	
protected:
	UPROPERTY(Transient)
	UChatComponent* ChatComponent;

	//////////////////////////////////////////////////////////////////////////
	/// Chat Channel
public:
	FChatChannel* GetChatChannel() const { return ChatChannel; }
	bool ToggleChatChannel(FChatChannel* InChatChannel);

	void ToggleToReplayWithPlayerID(int32 PlayerID);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateChannelText(const FText& InChannelText, const FLinearColor& InDisplayColor);

protected:
	FChatChannel* ChatChannel;

	//////////////////////////////////////////////////////////////////////////
	/// Chat input
public:
	UFUNCTION(BlueprintCallable)
	void FocusToChatInput();
	UFUNCTION(BlueprintCallable)
	void FocusToChatCommand();
	UFUNCTION(BlueprintCallable)
	void FocusToChatReply();

protected:
	UPROPERTY(Transient, BlueprintReadWrite)
	UChatInput* ChatInput;
	//////////////////////////////////////////////////////////////////////////
	/// Chat Line
public:
	void ReceiveChatMessage(const FChatChannel* InChatChannel, const FChatMessageInfo& ChatMessage);

	UFUNCTION(BlueprintImplementableEvent)
	void AddChatLine(class UChatLine* ChatLine);
};
