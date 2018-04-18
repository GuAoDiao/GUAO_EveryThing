// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreItem.h"

#include "Online/EveryThingPlayerState_Game.h"

void UScoreItem::InitializeScoreItem(AEveryThingPlayerState_Game* TargetETPS)
{
	if (TargetETPS)
	{
		InitializeScoreItemDisplay(TargetETPS->GetPlayerInfo().PlayerName, TargetETPS->GetTeamID());
		
		UpdatePlayerGameScoreDisplay(TargetETPS->GetGameScore());
		UpdatePlayerKillNumDisplay(TargetETPS->GetKillNum());
		UpdatePlayerDeathNumDisplay(TargetETPS->GetDeathNum());

		TargetETPS->OnGameScoreUpdateDelegate.AddUObject(this, &UScoreItem::UpdatePlayerGameScoreDisplay);
		TargetETPS->OnKillNumUpdateDelegate.AddUObject(this, &UScoreItem::UpdatePlayerKillNumDisplay);
		TargetETPS->OnDeathNumUpdateDelegate.AddUObject(this, &UScoreItem::UpdatePlayerDeathNumDisplay);
	}
}

