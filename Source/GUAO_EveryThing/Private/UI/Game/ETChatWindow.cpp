// Fill out your copyright notice in the Description page of Project Settings.

#include "ETChatWindow.h"

#include "Package.h"
#include "Engine/World.h"

#include "Online/EveryThingGameState_Game.h"
#include "Online/EveryThingPlayerState_Game.h"

#include "Online/PlayerController_Game.h"
#include "ChatWindow/ChatWindowTypes.h"
#include "ChatWindow/ChatComponent.h"
#include "ChatWindow/ChatWindowControllerInterface.h"
#include "ChatWindow/Channel/SystemChatChannel.h"
#include "ChatWindow/ChatWindowPlayerStateInterface.h"

#include "Characters/GamePawn.h"

#define LOCTEXT_NAMESPACE "EveryThing_GmaeUI_ETChatWindow"

void UETChatWindow::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPC_G)
	{	
		OwnerPC_G->OnRoleNameUpdateDelegate.AddUObject(this, &UETChatWindow::OnRoleNameUpdate);
		OnRoleNameUpdate(OwnerPC_G->GetCurrentRoleName());
	}
}

void UETChatWindow::OnRoleNameUpdate(const FName& RoleName)
{
	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	AGamePawn* OwnerGamePawn = OwnerPC_G ? Cast<AGamePawn>(OwnerPC_G->GetPawn()) : nullptr;
	if (OwnerGamePawn)
	{
		OwnerGamePawn->OnToggleToTargetFormFailureDelegate.AddUObject(this, &UETChatWindow::OnToggleToTargetFormFailure);
		OwnerGamePawn->OnToggleToTargetFormSuccessDelegate.AddUObject(this, &UETChatWindow::OnToggleToTargetFormSuccess);

		OwnerGamePawn->OnToggleToTargetSkinFailureDelegate.AddUObject(this, &UETChatWindow::OnToggleToTargetSkinFailure);
		OwnerGamePawn->OnToggleToTargetSkinSuccessDelegate.AddUObject(this, &UETChatWindow::OnToggleToTargetSkinSuccess);
	}

}

void UETChatWindow::OnToggleToTargetSkinSuccess(const FName& TargetSkinName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("TargetSkinName"), FText::FromName(TargetSkinName));

	FText DisplayInfo = FText::Format(LOCTEXT("OnToggleToTargetSkinSuccess", "Success toggle toggle to target skin : {TargetSkinName}."), Arguments);

	AddSystemMessage(ESystemMessageType::Success, DisplayInfo);
}


void UETChatWindow::OnToggleToTargetSkinFailure(const FName& TargetSkinName, const FText& ErrorInfo)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("TargetSkinName"), FText::FromName(TargetSkinName));
	Arguments.Add(TEXT("ErrorInfo"), ErrorInfo);


	FText DisplayInfo = FText::Format(LOCTEXT("OnToggleToTargetSkinFailure", "Failure toggle to target skin : {TargetSkinName}, because: {ErrorInfo}"), Arguments);

	AddSystemMessage(ESystemMessageType::Error, DisplayInfo);
}

void UETChatWindow::OnToggleToTargetFormSuccess(const FName& TargetFormName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("TargetFormName"), FText::FromName(TargetFormName));

	FText DisplayInfo = FText::Format(LOCTEXT("OnToggleToTargetFormSuccess", "Success toggle toggle to target form : {TargetFormName}."), Arguments);

	AddSystemMessage(ESystemMessageType::Success, DisplayInfo);
}


void UETChatWindow::OnToggleToTargetFormFailure(const FName& TargetFormName, const FText& ErrorInfo)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("TargetFormName"), FText::FromName(TargetFormName));
	Arguments.Add(TEXT("ErrorInfo"), ErrorInfo);


	FText DisplayInfo = FText::Format(LOCTEXT("OnToggleToTargetFormFailure", "Failure toggle to target form : {TargetFormName}, because: {ErrorInfo}"), Arguments);

	AddSystemMessage(ESystemMessageType::Error, DisplayInfo);
}





void UETChatWindow::OnPlayerJoinGame(APlayerState* InPlayerState)
{
	IChatWindowPlayerStateInterface* TargetCWPS = Cast<IChatWindowPlayerStateInterface>(InPlayerState);
	if (TargetCWPS)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("PlayerName"), FText::FromString(TargetCWPS->GetPlayerChatName()));
		FText DisplayInfo = FText::Format(LOCTEXT("OnPlayerJoinGame", "Welcome the Player {PlayerName}."), Arguments);

		AddSystemMessage(ESystemMessageType::Normal, DisplayInfo, false);
	}
}
void UETChatWindow::OnPlayerLeaveGame(APlayerState* InPlayerState)
{
	IChatWindowPlayerStateInterface* TargetCWPS = Cast<IChatWindowPlayerStateInterface>(InPlayerState);
	if (TargetCWPS)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("PlayerName"), FText::FromString(TargetCWPS->GetPlayerChatName()));
		FText DisplayInfo = FText::Format(LOCTEXT("OnPlayerLeaveGame", "The Player {PlayerName} have left the game"), Arguments);

		AddSystemMessage(ESystemMessageType::Normal, DisplayInfo, false);
	}
}


void UETChatWindow::AddSystemMessage(ESystemMessageType SystemMessageType, const FText& MessageText, bool bIsLocal)
{
	if (ChatComponent)
	{
		FSystemChatChannel* SystemChatChannel = static_cast<FSystemChatChannel*>(ChatComponent->GetChannelManager().GetChatchannelDefaultValueFromName("System"));
		if (SystemChatChannel)
		{
			SystemChatChannel->SetMessageType(SystemMessageType);
			FChatMessageInfo MessageInfo(MessageText.ToString(), SystemChatChannel->GetDisplayLineColor());
			if (bIsLocal)
			{
				ChatComponent->ReceiveChatMessage(SystemChatChannel, MessageInfo);
			}
			else
			{
				ChatComponent->CreateChatMessage(SystemChatChannel, MessageInfo);
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE