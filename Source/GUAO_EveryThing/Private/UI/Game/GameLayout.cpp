// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLayout.h"


#include "UI/Game/SelectAboutRole/SelectRolesBox.h"
#include "UI/Game/SelectAboutRole/SelectSkinsBox.h"
#include "UI/Game/SelectAboutRole/SelectFormsBox.h"
#include "UI/Game/PlayerFightInfo.h"


//////////////////////////////////////////////////////////////////////////
/// About Role Select
void UGameLayout::DisplaySelectRolesBox() { if (SelectRolesBox) { SelectRolesBox->SetVisibility(ESlateVisibility::Visible); } }
void UGameLayout::RemoveSelectRolesBox() { if (SelectRolesBox) { SelectRolesBox->SetVisibility(ESlateVisibility::Hidden); } }

void UGameLayout::DisplaySelectSkinsBox() { if (SelectSkinsBox) { SelectSkinsBox->SetVisibility(ESlateVisibility::Visible); } }
void UGameLayout::RemoveSelectSkinsBox() { if (SelectSkinsBox) { SelectSkinsBox->SetVisibility(ESlateVisibility::Hidden); } }

void UGameLayout::DisplaySelectFormsBox() { if (SelectFormsBox) { SelectFormsBox->SetVisibility(ESlateVisibility::Visible); } }
void UGameLayout::RemoveSelectFormsBox() { if (SelectFormsBox) { SelectFormsBox->SetVisibility(ESlateVisibility::Hidden); } }

//////////////////////////////////////////////////////////////////////////
/// Player Fight Info
void UGameLayout::ClientCreatePlayerFightInfo(const FText& FightInfo) { if (PlayerFightInfo) { PlayerFightInfo->ClientCreatePlayerFightInfo(FightInfo); } }