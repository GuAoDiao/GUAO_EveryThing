// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingPlayerState_House.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"

#include "Online/EveryThingGameState_House.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "EveryThingGameInstance.h"

AEveryThingPlayerState_House::AEveryThingPlayerState_House()
{
	bIsReady = false;
	TeamID = -1;
}

void AEveryThingPlayerState_House::BeginPlay()
{
	Super::BeginPlay();

	// update player info from GameInstance.
	UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
	APlayerController* OwnerPC = Cast<APlayerController>(GetOwner());
	if (OwnerETGI && OwnerPC && OwnerPC->IsLocalController() && !bFromPreviousLevel)
	{
		ServerSetPlayerInfo(OwnerETGI->GetPlayerInfo());
	}

	if (HasAuthority())
	{
		AEveryThingGameState_House* OwnerETGS_H = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_House>() : nullptr;
		if (OwnerETGS_H)
		{
			OnAllowedTeamNumChanged(OwnerETGS_H->GetAllowedTeamNum());
			OwnerETGS_H->OnAllowedTeamNumChangeDelegate.AddUObject(this, &AEveryThingPlayerState_House::OnAllowedTeamNumChanged);
		}
	}
}


void AEveryThingPlayerState_House::SeamlessTravelTo(class APlayerState* NewPlayerState)
{
	Super::SeamlessTravelTo(NewPlayerState);

	// move old info to new
	AEveryThingPlayerState_Game* OldETPS_G = Cast<AEveryThingPlayerState_Game>(NewPlayerState);
	if (OldETPS_G)
	{
		OldETPS_G->SetPlayerInfo(CurrentPlayerInfo);
		OldETPS_G->ChangeTeamID(TeamID);
	}
}


//////////////////////////////////////////////////////////////////////////
/// Player Info
void AEveryThingPlayerState_House::SetPlayerInfo(const FPlayerInfo& InPlayerInfo) { CurrentPlayerInfo = InPlayerInfo; OnCurrentPlayerInfoUpdate(); }
bool AEveryThingPlayerState_House::ServerSetPlayerInfo_Validate(const FPlayerInfo& InPlayerInfo) { return true; }
void AEveryThingPlayerState_House::ServerSetPlayerInfo_Implementation(const FPlayerInfo& InPlayerInfo)
{
	SetPlayerInfo(InPlayerInfo);
}

//////////////////////////////////////////////////////////////////////////
/// Is Ready
void AEveryThingPlayerState_House::SetIsReady(bool bInIsReady)
{
	bIsReady = bInIsReady;
	OnIsReadyUptate();
}
void AEveryThingPlayerState_House::TooggleReadState()
{
	if (GetOwner() && GetOwner()->Role >= ROLE_AutonomousProxy){ServerTooggleReadState();}
}
bool AEveryThingPlayerState_House::ServerTooggleReadState_Validate() { return true; }
void AEveryThingPlayerState_House::ServerTooggleReadState_Implementation() { SetIsReady(!bIsReady); }


//////////////////////////////////////////////////////////////////////////
/// Team ID

void AEveryThingPlayerState_House::OnAllowedTeamNumChanged(int32 AllowedTeamNum)
{
	if (TeamID > 0 && TeamID <= AllowedTeamNum)
	{

	}
	else
	{
		AEveryThingGameState_House* OwnerETGS_H = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_House>() : nullptr;
		int32 NewTeamID = OwnerETGS_H ? OwnerETGS_H->GetRandomTeamID() : 1;
		ChangeTeamID(NewTeamID);
	}
}

void AEveryThingPlayerState_House::ChangeTeamID(int32 InTeamID)
{
	AEveryThingGameState_House* OwnerETGS_H = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_House>() : nullptr;
	if (OwnerETGS_H && OwnerETGS_H->CheckTeamIDIsAllowed(InTeamID))
	{
		if (!HasAuthority() && GetOwner() && GetOwner()->Role >= ROLE_AutonomousProxy)
		{
			ServerChangeTeamID(InTeamID);
		}
		else
		{
			SetTeamID(InTeamID);
		}
	}
}
bool AEveryThingPlayerState_House::ServerChangeTeamID_Validate(int32 InTeamID) { return true; }
void AEveryThingPlayerState_House::ServerChangeTeamID_Implementation(int32 InTeamID) { ChangeTeamID(InTeamID); }


bool AEveryThingPlayerState_House::CheckIsHouseOwner()
{
	AEveryThingGameState_House* OwnerETGS_H = GetWorld() ? GetWorld()->GetGameState<AEveryThingGameState_House>() : nullptr;
	return OwnerETGS_H && OwnerETGS_H->CheckPlayerIsHouseOwner(this);
}

void AEveryThingPlayerState_House::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEveryThingPlayerState_House, CurrentPlayerInfo);
	DOREPLIFETIME(AEveryThingPlayerState_House, bIsReady);
	DOREPLIFETIME(AEveryThingPlayerState_House, TeamID);
}