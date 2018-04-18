// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreBox.h"

#include "UI/Game/GameInfo/ScoreItem.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameState_Game.h"
#include "EveryThingAssetManager.h"



void UScoreBoard::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (World)
	{
		AEveryThingGameState_Game* OwnerETGS_G = World->GetGameState<AEveryThingGameState_Game>();
		if (OwnerETGS_G)
		{
			SetSectionNums(OwnerETGS_G->GetActualTeamNums());
			for (APlayerState* PlayerState : OwnerETGS_G->PlayerArray)
			{
				OnPlayerStateAdd(PlayerState);
			}

			OwnerETGS_G->OnPlayerStateAddDelegate.AddUObject(this, &UScoreBoard::OnPlayerStateAdd);
			OwnerETGS_G->OnPlayerStateRemoveDelegate.AddUObject(this, &UScoreBoard::OnPlayerStateRemove);
		}
	}
}

void UScoreBoard::OnPlayerStateAdd(class APlayerState* NewPS)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	AEveryThingPlayerState_Game* NewETPS_G = Cast<AEveryThingPlayerState_Game>(NewPS);
	TSubclassOf<UUserWidget> ScoreItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("ScoreItem");

	if (NewETPS_G && OwnerPC && ScoreItemClass)
	{
		UScoreItem* ScoreItem = CreateWidget<UScoreItem>(OwnerPC, ScoreItemClass);
		if(ScoreItem)
		{
			ScoreItem->InitializeScoreItem(NewETPS_G);

			int32 SectionID = NewETPS_G->GetTeamID();
			if (SectionID == -1) { SectionID = 1; }

			AddScoreItem(ScoreItem, SectionID);

			AllScoreItem.Add(NewPS, ScoreItem);
		}
	}
}

void UScoreBoard::OnPlayerStateRemove(class APlayerState* RemovePS)
{
	AEveryThingPlayerState_Game* RemvoeETPS_G = Cast<AEveryThingPlayerState_Game>(RemovePS);
	if (RemvoeETPS_G && AllScoreItem.Contains(RemovePS))
	{
		RemoveScoreItem(AllScoreItem[RemovePS], RemvoeETPS_G->GetTeamID());
	}
}
