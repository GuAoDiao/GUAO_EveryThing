// Fill out your copyright notice in the Description page of Project Settings.

#include "AllChatChannel.h"


FAllChatChannel::FAllChatChannel()
{
	ChannelName = "All";
	DisplayNameTextFormat = TEXT("%s Says : ");
}

FLinearColor FAllChatChannel::GetDisplayLineColor() const { return FLinearColor(0.f, 0.f, 1.f); }
FLinearColor FAllChatChannel::GetDisplayChannelColor() const { return FLinearColor(0.f, 0.f, 1.f); }