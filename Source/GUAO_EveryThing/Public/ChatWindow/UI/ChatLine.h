// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatLine.generated.h"

class UChatWindow;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UChatLine : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitalizeChatLine(UChatWindow* InChatWindow, const FName& InChatChannelName, const FText& InChanenelText, int32 InPlayerID, const FText& InNameText, const FText& InMessageText, const FLinearColor& InDisplayColor);

	UFUNCTION(BlueprintCallable)
	void OnClickedName();

	UFUNCTION(BlueprintCallable)
	void OnClickedChannel();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTimeStamp(const FText& TimeStampText);

	int32 PlayerID;
	FName ChatChannelName;

	UChatWindow* ChatWindow;
};
