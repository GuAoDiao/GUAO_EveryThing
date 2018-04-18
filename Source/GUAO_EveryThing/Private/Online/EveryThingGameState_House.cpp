// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameState_House.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"

#include "EveryThingAssetManager.h"
#include "Online/EveryThingPlayerState_House.h"

void AEveryThingGameState_House::BeginPlay()
{
	Super::BeginPlay();

	// get HouseOwner, now select the first PlayerController.
	if (HasAuthority() && GetWorld())
	{
		APlayerController* OwnerPC = GetWorld()->GetFirstPlayerController();
		HouseOwner = OwnerPC ? Cast<AEveryThingPlayerState_House>(OwnerPC->PlayerState) : nullptr;
		if (HouseOwner) { HouseOwner->SetIsReady(true); }
	}
}

void AEveryThingGameState_House::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	OnAddPlayerDelegate.Broadcast(PlayerState);
}

void AEveryThingGameState_House::RemovePlayerState(APlayerState* PlayerState)
{
	OnRemovePlayerDelegate.Broadcast(PlayerState);
	Super::RemovePlayerState(PlayerState);
}

bool AEveryThingGameState_House::CheckIsAllPlayerAreReady()
{	
	for (APlayerState* Player : PlayerArray)
	{
		AEveryThingPlayerState_House* CurrentETPS_H = Cast<AEveryThingPlayerState_House>(Player);
		if (!CurrentETPS_H) { return false; }
		if (!CurrentETPS_H->CheckIsHouseOwner() && !CurrentETPS_H->GetIsReady()) { return false; }
	}

	return true;
}

bool AEveryThingGameState_House::CheckHaveEnoughTeams()
{
	if (MapTypeInfo)
	{
		if (MapTypeInfo->NeededTeamNum == 0) { return true; }

		TArray<int32> AllTeamPlayerNums;
		int32 NeeedTemNum  = MapTypeInfo->NeededTeamNum;

		for (APlayerState* PlayerState : PlayerArray)
		{
			AEveryThingPlayerState_House* OwnerETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);
			if (OwnerETPS_H)
			{
				int32 TeamID = OwnerETPS_H->GetTeamID();
				if (CheckTeamIDIsAllowed(TeamID) && !AllTeamPlayerNums.Contains(TeamID))
				{
					
					AllTeamPlayerNums.Add(TeamID);
					--NeeedTemNum;
					if (NeeedTemNum <= 0) { return true; }
				}
			}
		}
	}

	return false;
}

void AEveryThingGameState_House::OnHouseSettingUpdate()
{
	OnHouseSettingUpdateDelegate.Broadcast(HouseName, GameType, MapName, bIsLANMatch, MaxPlayerNum);

	if (HasAuthority()) { OnGameTypeChanged(); }
}

void AEveryThingGameState_House::OnGameTypeChanged()
{
	MapTypeInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetMapTypeInfoFromName(FName(*GameType));
	
	checkf(MapTypeInfo, TEXT("-_- the map type(%s) info must be exists."), *GameType);

	AllowedTeamNum = MapTypeInfo->AllowedTeamNum;

	OnAllowedTeamNumChanged();
}

int32 AEveryThingGameState_House::GetRandomTeamID() const
{
	if (AllowedTeamNum == 0) { return -1; }

	TArray<int32> AllTeamPlayerNums;
	for (int32 i = 0; i < AllowedTeamNum; ++i)
	{
		AllTeamPlayerNums.Add(0);
	}

	for (const APlayerState* PlayerState : PlayerArray)
	{
		const AEveryThingPlayerState_House* ETPS_H = Cast<AEveryThingPlayerState_House>(PlayerState);
		if (ETPS_H)
		{
			int32 TeamID = ETPS_H->GetTeamID();
			if (CheckTeamIDIsAllowed(TeamID)) { ++AllTeamPlayerNums[TeamID - 1]; }
		}
	}

	int32 PlayerNumMinTeam = 0;

	int32 MinPlayerNum = INT_MAX;
	for (int32 i = 0; i < AllTeamPlayerNums.Num(); ++i)
	{
		if (AllTeamPlayerNums[i] < MinPlayerNum)
		{
			PlayerNumMinTeam = i;
			MinPlayerNum = AllTeamPlayerNums[i];
		}
	}

	return PlayerNumMinTeam + 1;
}


void AEveryThingGameState_House::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingGameState_House, GameType);
	DOREPLIFETIME(AEveryThingGameState_House, MapName);
	DOREPLIFETIME(AEveryThingGameState_House, HouseName);
	DOREPLIFETIME(AEveryThingGameState_House, bIsLANMatch);
	DOREPLIFETIME(AEveryThingGameState_House, MaxPlayerNum);

	DOREPLIFETIME(AEveryThingGameState_House, AllowedTeamNum);
}