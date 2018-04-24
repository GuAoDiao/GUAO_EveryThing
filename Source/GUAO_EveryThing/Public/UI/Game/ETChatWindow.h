// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatWindow/UI/ChatWindow.h"

#include "EveryThingTypes.h"

#include "ETChatWindow.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UETChatWindow : public UChatWindow
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void OnRoleNameUpdate(const FName& RoleName);
		
	void OnToggleToTargetSkinSuccess(const FName& TargetSkinName);
	void OnToggleToTargetSkinFailure(const FName& TargetSkinName, const FText& ErrorInfo);

	void OnToggleToTargetFormSuccess(const FName& TargetFormName);
	void OnToggleToTargetFormFailure(const FName& TargetFormName, const FText& ErrorInfo);



	void OnPlayerJoinGame(class APlayerState* InPlayerState);
	void OnPlayerLeaveGame(class APlayerState* InPlayerState);

	void AddSystemMessage(ESystemMessageType SystemMessageType, const FText& MessageText, bool bIsLocal = true);
private:
	TArray<FKey> EnterKeys;
};
