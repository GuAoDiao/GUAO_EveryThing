// Fill out your copyright notice in the Description page of Project Settings.

#include "SystemChatChannel.h"

FSystemChatChannel::FSystemChatChannel()
{
	ChannelName = "System";

	DisplayChannelTextFormat = TEXT("[ %s ] : ");
}


FLinearColor FSystemChatChannel::GetDisplayLineColor() const
{
	switch (MessageType)
	{
		case ESystemMessageType::Normal:
			return FLinearColor(1.f, 1.f, 1.f, 1.f);
		case ESystemMessageType::Alert:
			return FLinearColor(0.9f, 0.81f, 0.21f, 1.f);
		case ESystemMessageType::Success:
			return FLinearColor(0.2f, 1.f, 0.2f, 1.f);
		case ESystemMessageType::Error:
			return FLinearColor(1.f, 0.f, 0.f, 1.f);
		case ESystemMessageType::Team:
			return FLinearColor(1.f, 0.5f, 0.1f, 1.f);
		case ESystemMessageType::Custom:
			return DisplayColor;
	}

	return FLinearColor::White;
}