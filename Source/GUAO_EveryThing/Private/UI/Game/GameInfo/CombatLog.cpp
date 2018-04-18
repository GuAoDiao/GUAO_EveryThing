// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatLog.h"

#include "SceneObject/HitAbleInterface.h"
#include "EveryThingAssetManager.h"
#include "Online/PlayerController_Game.h"
#include "Characters/GamePawn.h"
#include "UI/Game/GameInfo/CombatLogItem.h"

void UCombatLog::NativeConstruct()
{
	APlayerController_Game* OnwerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OnwerPC_G) { OnwerPC_G->OnGamePawnBeKilledDelegate.AddUObject(this, &UCombatLog::OnGamePawnBeKilled); }
	if (OnwerPC_G) { OnwerPC_G->OnGamePawnSuicidedDelegate.AddUObject(this, &UCombatLog::OnGamePawnSuicided); }
	
	Super::NativeConstruct();
}



void UCombatLog::OnGamePawnBeKilled(const FString& KilledName, const FString& KillerName)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	TSubclassOf<UUserWidget> CombatLogItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("CombatLogItem");
	if (OwnerPC && CombatLogItemClass)
	{
		UCombatLogItem* CombatLogItem = CreateWidget<UCombatLogItem>(OwnerPC, CombatLogItemClass);
		if (CombatLogItem)
		{
			CombatLogItem->InitializeCombatLogBeKilled(KilledName, KillerName);
			AddCombatLogItem(CombatLogItem);
		}
	}
}

void UCombatLog::OnGamePawnSuicided(const FString& KilledName)
{
	APlayerController* OwnerPC = GetOwningPlayer();
	TSubclassOf<UUserWidget> CombatLogItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("CombatLogItem");
	if (OwnerPC && CombatLogItemClass)
	{
		UCombatLogItem* CombatLogItem = CreateWidget<UCombatLogItem>(OwnerPC, CombatLogItemClass);
		if (CombatLogItem)
		{
			CombatLogItem->InitializeCombatLogSuicided(KilledName);
			AddCombatLogItem(CombatLogItem);
		}
	}
}