// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerFightInfo.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "VerticalBox.h"

#include "EveryThingAssetManager.h"
#include "Online/PlayerController_Game.h"
#include "UI/Game/PlayerFightInfoItem.h"

#define LOCTEXT_NAMESPACE "EveryThing_GmaeUI_PlayerFightInfo"

void UPlayerFightInfo::NativeConstruct()
{
	Super::NativeConstruct();

	TidyRate = 0.5f;
	InitializePlayerFightInfo();

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPC_G)
	{
		OwnerPC_G->OnAcceptCureDelegate.AddUObject(this, &UPlayerFightInfo::OnAcceptCure);
		OwnerPC_G->OnAcceptDamageDelegate.AddUObject(this, &UPlayerFightInfo::OnAcceptDamage);
		OwnerPC_G->OnAcceptCriticalDamageDelegate.AddUObject(this, &UPlayerFightInfo::OnAcceptCriticalDamage);
		OwnerPC_G->OnTakeCureDelegate.AddUObject(this, &UPlayerFightInfo::OnTakeCure);
		OwnerPC_G->OnTakeDamageDelegate.AddUObject(this, &UPlayerFightInfo::OnTakeDamage);
		OwnerPC_G->OnTakeCriticalDamageDelegate.AddUObject(this, &UPlayerFightInfo::OnTakeCriticalDamage);

		OwnerPC_G->OnBeKilledDelegate.AddUObject(this, &UPlayerFightInfo::OnBeKilled);
		OwnerPC_G->OnSuicidedDelegate.AddUObject(this, &UPlayerFightInfo::OnSuicided);
		OwnerPC_G->OnKillOtherDelegate.AddUObject(this, &UPlayerFightInfo::OnKillOther);
	}		
}


void UPlayerFightInfo::OnAcceptCure(const FString& CauserName, float Treatment)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("CauserName"), FText::FromString(CauserName));
	Arguments.Add(TEXT("Treatment"), Treatment);

	FText FightInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "{CauserName} Take {Treatment} Treatment To You."), Arguments);
	AddPlayerFightInfoItem(FightInfo);
}
void UPlayerFightInfo::OnAcceptDamage(const FString& CauserName, float Damage)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("CauserName"), FText::FromString(CauserName));
	Arguments.Add(TEXT("Damage"), Damage);

	FText FightInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "{CauserName} Take {Damage} Damage To You."), Arguments);
	AddPlayerFightInfoItem(FightInfo);
}
void UPlayerFightInfo::OnAcceptCriticalDamage(const FString& CauserName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("CauserName"), FText::FromString(CauserName));

	FText FightInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "{CauserName} Take Critical Damge To You."), Arguments);
	AddPlayerFightInfoItem(FightInfo);
}



void UPlayerFightInfo::OnTakeCure(const FString& AccepterName, float Treatment)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("Treatment"), Treatment);
	Arguments.Add(TEXT("AccepterName"), FText::FromString(AccepterName));

	FText FightInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "You Take {Treatment} Treatment To {AccepterName}."), Arguments);
	AddPlayerFightInfoItem(FightInfo);
}
void UPlayerFightInfo::OnTakeDamage(const FString& AccepterName, float Damage)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("Damage"), Damage);
	Arguments.Add(TEXT("AccepterName"), FText::FromString(AccepterName));

	FText FightInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "You Take {Damage} Damage To {AccepterName}."), Arguments);
	AddPlayerFightInfoItem(FightInfo);

}
void UPlayerFightInfo::OnTakeCriticalDamage(const FString& AccepterName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("AccepterName"), FText::FromString(AccepterName));

	FText FightInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "You Take Critical Damage To {AccepterName}."), Arguments);
	AddPlayerFightInfoItem(FightInfo);
}


void UPlayerFightInfo::OnBeKilled(const FString& KillerName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("KillerName"), FText::FromString(KillerName));

	FText FightInfo = FText::Format(LOCTEXT("CombatLogOnHaveKillerName", "{KillerName} Kill You."), Arguments);
	AddPlayerFightInfoItem(FightInfo);
}
void UPlayerFightInfo::OnSuicided()
{
	AddPlayerFightInfoItem(LOCTEXT("CombatLogOnHaveKillerName", "You Suicided."));
}
void UPlayerFightInfo::OnKillOther(const FString& KilledName)
{
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("KilledName"), FText::FromString(KilledName));

	FText FightInfo = FText::Format(LOCTEXT("PlayerFightInfoOnKillOther", "You Kill {KilledName}."), Arguments);
	AddPlayerFightInfoItem(FightInfo);
}

void UPlayerFightInfo::ClientCreatePlayerFightInfo(const FText& FightInfo) { AddPlayerFightInfoItem(FightInfo); }
void UPlayerFightInfo::AddPlayerFightInfoItem(const FText& FightInfo, float InRamainingTime)
{
	if (PlayerFightInfoVerticalBox)
	{
		TSubclassOf<UUserWidget> PlayerFightInfoItemClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetUserWidgetFromName("PlayerFightInfoItem");
		UPlayerFightInfoItem* PlayerFightInfoItem = PlayerFightInfoItemClass ? CreateWidget<UPlayerFightInfoItem>(GetOwningPlayer(), PlayerFightInfoItemClass) : nullptr;
		if (PlayerFightInfoItem)
		{
			PlayerFightInfoItem->InitializePlayerFightInfoItemDisplay(FightInfo);
			PlayerFightInfoItem->RemainingTime = InRamainingTime;

			PlayerFightInfoVerticalBox->AddChild(PlayerFightInfoItem);			
		}
	}

	if (GetWorld())
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (!TimerManager.IsTimerActive(TidyTimer))
		{
			TimerManager.SetTimer(TidyTimer, this, &UPlayerFightInfo::TidyPlayerFightInfo, TidyRate, true);
		}
	}

	ScrollVerticalBoxToEnd();
}

void UPlayerFightInfo::TidyPlayerFightInfo()
{
	if (PlayerFightInfoVerticalBox)
	{
		for (int32 i = 0, l = PlayerFightInfoVerticalBox->GetChildrenCount(); i < l; ++i)
		{
			UPlayerFightInfoItem* PlayerFightInfoItem = Cast<UPlayerFightInfoItem>(PlayerFightInfoVerticalBox->GetChildAt(i));
			if (PlayerFightInfoItem)
			{
				PlayerFightInfoItem->RemainingTime -= TidyRate;
				if (PlayerFightInfoItem->RemainingTime <= 0.f)
				{
					PlayerFightInfoVerticalBox->RemoveChildAt(i);
					i--;
					l--;
				}
			}
		}

		if (PlayerFightInfoVerticalBox->GetChildrenCount() == 0 && GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TidyTimer);
		}
	}
}



#undef LOCTEXT_NAMESPACE