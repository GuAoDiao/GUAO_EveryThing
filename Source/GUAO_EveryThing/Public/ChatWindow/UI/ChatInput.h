// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatInput.generated.h"


class UChatWindow;

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeChatInput(UChatWindow* InChatWindow);


	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FocusToInputText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	bool HasFocusToInputText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInputText(const FText& InText);

	UFUNCTION(BlueprintCallable)
	void ProcessInput(const FText& InputText);

	UFUNCTION(BlueprintCallable)
	void ExcuteInput(const FText& InputText);

	UFUNCTION(BlueprintPure)
	bool IsCommand(const FText& Command) const;

private:
	UPROPERTY(Transient)
	UChatWindow* ChatWindow;
};
