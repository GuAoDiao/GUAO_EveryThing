// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameState_Game.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "TimerManager.h"

#include "Characters/GamePawn.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameMode_Game.h"
#include "Online/PlayerController_Game.h"
#include "EveryThingAssetManager.h"

const FString AEveryThingGameState_Game::PlayerChatName_NONE = FString("NONE");

AEveryThingGameState_Game::AEveryThingGameState_Game()
{
	CurrentPlayerNum = 0;
	bIsETGameStarted = false;
	CurrentETGameState = EETGameState::WaitForHousePlayerLoad;

	DefaultReadyTime = 15.f;
	DefaultGameTime = 600.f;
	DefaultBackToHouseTime = 10.f;

	ActualTeamNums = 1;
}

//////////////////////////////////////////////////////////////////////////
/// Game flow path
void AEveryThingGameState_Game::InitializeETGameState(const FString& InGameType, const FString& InMapName, const FString& InHouseName, bool bInIsLANMatach, int32 InMaxPlayerNum, int32 InCurrentPlayerNum)
{
	GameType = InGameType;
	MapName = InMapName;
	HouseName = InHouseName;
	bIsLANMatach = bInIsLANMatach;
	MaxPlayerNum = InMaxPlayerNum;
	HousePlayerNum = InCurrentPlayerNum;

	OnGameTypeChanged();
}


void AEveryThingGameState_Game::OnGameTypeChanged()
{
	MapTypeInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetMapTypeInfoFromName(FName(*GameType));

	checkf(MapTypeInfo, TEXT("-_- the map type(%s) info must be exists."), *GameType);

	AllowedTeamNum = MapTypeInfo->AllowedTeamNum;
	ActualTeamNums = MapTypeInfo->ActualTeamNums;

	OnAllowedTeamNumChanged();

	CureScoreScale = MapTypeInfo->CureScoreScale;
	DamageScoreScale = MapTypeInfo->DamageScoreScale;
	CriticalDamageScore = MapTypeInfo->CriticalDamageScore;
	KillScore = MapTypeInfo->KillScore;
}

int32 AEveryThingGameState_Game::GetRandomTeamID() const
{
	if (AllowedTeamNum == 0) { return -1; }

	TArray<int32> AllTeamPlayerNums;
	for (int32 i = 0; i < AllowedTeamNum; ++i)
	{
		AllTeamPlayerNums.Add(0);
	}

	for (const APlayerState* PlayerState : PlayerArray)
	{
		const AEveryThingPlayerState_Game* ETPS_G = Cast<AEveryThingPlayerState_Game>(PlayerState);
		if (ETPS_G)
		{
			int32 TeamID = ETPS_G->GetTeamID();
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


//////////////////////////////////////////////////////////////////////////
/// Start Ready
void AEveryThingGameState_Game::StartReadyCountDown()
{
	RemaningReadyTime = DefaultReadyTime;
	OnRemaningReadyTimeUpdate();

	GetWorldTimerManager().SetTimer(CountDownTimer, this, &AEveryThingGameState_Game::ReadyCountDown, 1.f, true);
}

void AEveryThingGameState_Game::ReadyCountDown()
{
	if (RemaningReadyTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimer);

		ToggleToTargetETGameState(EETGameState::Gameing);
	}
	else
	{
		RemaningReadyTime -= 1.f;
		OnRemaningReadyTimeUpdate();
	}
}


/// Game

void AEveryThingGameState_Game::StartGameTimeCountDown()
{
	RemaningGameTime = DefaultGameTime;
	OnRemaningGameTimeUpdate();

	// TODO: Need to optimize
	GetWorldTimerManager().SetTimer(CountDownTimer, this, &AEveryThingGameState_Game::GameTimeCountDown, 1.f, true);
}
void AEveryThingGameState_Game::GameTimeCountDown()
{
	if (RemaningGameTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimer);

		ToggleToTargetETGameState(EETGameState::GameOver);
	}
	else
	{
		RemaningGameTime -= 1.f;
		OnRemaningGameTimeUpdate();
	}
}

/// Back to house
void AEveryThingGameState_Game::StartBackToHouseTimeCountDown()
{
	RemaningBackToHouseTime = DefaultBackToHouseTime;
	OnRemaningBackToHouseTimeUpdate();

	GetWorldTimerManager().SetTimer(CountDownTimer, this, &AEveryThingGameState_Game::BackToHouseeTimeCountDown, 1.f, true);
}

void AEveryThingGameState_Game::BackToHouseeTimeCountDown()
{
	if (RemaningBackToHouseTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimer);

		AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
		if (OwnerETGM_G) { OwnerETGM_G->HandlerETBackToHouse(); }
	}
	else
	{
		RemaningBackToHouseTime -= 1.f;
		OnRemaningBackToHouseTimeUpdate();
	}
}

//////////////////////////////////////////////////////////////////////////
/// Game State

void AEveryThingGameState_Game::OnPlayerPostLogin(APlayerController* NewPlayer)
{
	++CurrentPlayerNum;

	UE_LOG(LogTemp, Log, TEXT("-_- On Player Add, Current Player: %d / %d"), CurrentPlayerNum, HousePlayerNum);

	if (IsTargetETGameState(EETGameState::WaitForHousePlayerLoad) && CurrentPlayerNum >= HousePlayerNum)
	{
		ToggleToTargetETGameState(EETGameState::ReadyToStart);
	}
}

void AEveryThingGameState_Game::OnPlayerLogout(AController* Exiting)
{
	--CurrentPlayerNum;

	if (IsTargetETGameState(EETGameState::WaitForHousePlayerLoad)) { --HousePlayerNum; }
}



void AEveryThingGameState_Game::ToggleToTargetETGameState(EETGameState TargetGameStae)
{
	CurrentETGameState = TargetGameStae;

	AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
	if (OwnerETGM_G)
	{
		switch (TargetGameStae)
		{
			case EETGameState::WaitForHousePlayerLoad:
				break;
			case EETGameState::ReadyToStart: {OwnerETGM_G->HandleETGameReay(); break; }
			case EETGameState::Gameing:
			{
				bIsETGameStarted = true;
				OwnerETGM_G->HandleETGameStart(); break;
			}
			case EETGameState::GameOver: {OwnerETGM_G->HandleETGameOver(); break; }
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Chat
const FString& AEveryThingGameState_Game::GetPlayerChatName(int32 PlayerID) const
{	
	AEveryThingPlayerState_Game* TargetETPS = ChatPlayerState.IsValidIndex(PlayerID) ? Cast<AEveryThingPlayerState_Game>(ChatPlayerState[PlayerID]) : nullptr;
	if (TargetETPS)
	{
		return TargetETPS->GetPlayerChatName();
	}

	return PlayerChatName_NONE;
}


//////////////////////////////////////////////////////////////////////////
/// Game Pawn Damage And Death
void AEveryThingGameState_Game::OnGamePawnAcceptCure(AGamePawn* AccpetPawn, AActor* Causer, float Treatment)
{
	APlayerController_Game* AcceptPC_G = AccpetPawn ? Cast<APlayerController_Game>(AccpetPawn->GetController()) : nullptr;
	if (AcceptPC_G)
	{
		IHitAbleInterface* CauserHitable = Cast<IHitAbleInterface>(Causer);
		FString CauserActorDisplayName = CauserHitable ? CauserHitable->GetHitAbleActorDisplayName() : TEXT("SceneObject"); 
		AcceptPC_G->ClientOnAcceptCure(CauserActorDisplayName, Treatment);
	}
	
	AGamePawn* CauserGamePawn = Cast<AGamePawn>(Causer);
	APlayerController_Game* CasuerPC_G = CauserGamePawn ? Cast<APlayerController_Game>(CauserGamePawn->GetController()) : nullptr;
	if (CasuerPC_G)
	{
		AEveryThingPlayerState_Game* CauserETPS_G = Cast<AEveryThingPlayerState_Game>(CasuerPC_G->PlayerState);
		if (CauserETPS_G) { CauserETPS_G->AddGameScore(Treatment * CureScoreScale); }

		if (AccpetPawn) { CasuerPC_G->ClientOnTakeCure(AccpetPawn->GetHitAbleActorDisplayName(), Treatment); }
	}
}
void AEveryThingGameState_Game::OnGamePawnAcceptDamage(AGamePawn* AccpetPawn, AActor* Causer, float Damage)
{
	APlayerController_Game* AcceptPC_G = AccpetPawn ? Cast<APlayerController_Game>(AccpetPawn->GetController()) : nullptr;
	if (AcceptPC_G)
	{
		IHitAbleInterface* CauserHitable = Cast<IHitAbleInterface>(Causer);
		FString CauserActorDisplayName = CauserHitable ? CauserHitable->GetHitAbleActorDisplayName() : TEXT("SceneObject");
		AcceptPC_G->ClientOnAcceptDamage(CauserActorDisplayName, Damage);
	}

	AGamePawn* CauserGamePawn = Cast<AGamePawn>(Causer);
	APlayerController_Game* CasuerPC_G = CauserGamePawn ? Cast<APlayerController_Game>(CauserGamePawn->GetController()) : nullptr;
	if (CasuerPC_G)
	{
		AEveryThingPlayerState_Game* CauserETPS_G = Cast<AEveryThingPlayerState_Game>(CasuerPC_G->PlayerState);
		if (CauserETPS_G) { CauserETPS_G->AddGameScore(Damage * DamageScoreScale); }

		if (AccpetPawn) { CasuerPC_G->ClientOnTakeDamage(AccpetPawn->GetHitAbleActorDisplayName(), Damage); }
	}
}

void AEveryThingGameState_Game::OnGamePawnAcceptCriticalDamage(AGamePawn* AccpetPawn, AActor* Causer)
{

	APlayerController_Game* AcceptPC_G = AccpetPawn ? Cast<APlayerController_Game>(AccpetPawn->GetController()) : nullptr;
	if (AcceptPC_G)
	{
		IHitAbleInterface* CauserHitable = Cast<IHitAbleInterface>(Causer);
		FString CauserActorDisplayName = CauserHitable ? CauserHitable->GetHitAbleActorDisplayName() : TEXT("SceneObject"); 
		AcceptPC_G->ClientOnAcceptCriticalDamage(CauserActorDisplayName);
	}

	AGamePawn* CauserGamePawn = Cast<AGamePawn>(Causer);
	APlayerController_Game* CasuerPC_G = CauserGamePawn ? Cast<APlayerController_Game>(CauserGamePawn->GetController()) : nullptr;
	if (CasuerPC_G)
	{
		AEveryThingPlayerState_Game* CauserETPS_G = Cast<AEveryThingPlayerState_Game>(CasuerPC_G->PlayerState);
		if (CauserETPS_G) { CauserETPS_G->AddGameScore(CriticalDamageScore); }

		if (AccpetPawn) { CasuerPC_G->ClientOnTakeCriticalDamage(AccpetPawn->GetHitAbleActorDisplayName()); }
	}
}

void AEveryThingGameState_Game::OnGamePawnBeKilled(AGamePawn* KilledGamePawn, AActor* KillerActor)
{
	AGamePawn* KillerGamePawn = Cast<AGamePawn>(KillerActor);

	APlayerController_Game* KilledPC_G = KilledGamePawn ? Cast<APlayerController_Game>(KilledGamePawn->GetController()) : nullptr;
	if (KilledPC_G)
	{
		AEveryThingPlayerState_Game* KilledETPS_G = Cast<AEveryThingPlayerState_Game>(KilledPC_G->PlayerState);
		if (KilledETPS_G) { KilledETPS_G->IncDeathNum(); }

		if (KillerActor)
		{
			IHitAbleInterface* KillerHitable = Cast<IHitAbleInterface>(KillerActor);
			FString KillerName = KillerHitable ? KillerHitable->GetHitAbleActorDisplayName() : TEXT("SceneObject");
			KilledPC_G->ClientOnBeKilled(KillerName);
		}
		else
		{
			KilledPC_G->ClientOnSuicided();
		}

		KilledPC_G->UnPossess();
		KilledPC_G->DelayToRestartGamePawn(3.f);
	}

	APlayerController_Game* KillerPC_G = KillerGamePawn ? Cast<APlayerController_Game>(KillerGamePawn->GetController()) : nullptr;
	if (KillerPC_G)
	{
		AEveryThingPlayerState_Game* KillerETPS_G = Cast<AEveryThingPlayerState_Game>(KillerPC_G->PlayerState);
		if (KillerETPS_G)
		{
			KillerETPS_G->AddGameScore(KillScore);
			KillerETPS_G->IncKillNum();
		}

		if (KillerGamePawn) { KillerPC_G->ClientOnKillOther(KillerGamePawn->GetHitAbleActorDisplayName()); }
	}

	if (GetWorld())
	{
		if (KillerGamePawn)
		{
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(It->Get());
				if (OwnerPC_G) { OwnerPC_G->ClientOnGamePawnBeKilled(KilledGamePawn->GetHitAbleActorDisplayName(), KillerGamePawn->GetHitAbleActorDisplayName()); }
			}
		}
		else
		{
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(It->Get());
				if (OwnerPC_G) { OwnerPC_G->ClientOnGamePawnSuicided(KilledGamePawn->GetHitAbleActorDisplayName()); }
			}
		}
	}
}


void AEveryThingGameState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(AEveryThingGameState_Game, CurrentETGameState);
	DOREPLIFETIME(AEveryThingGameState_Game, bIsETGameStarted);
	
	DOREPLIFETIME(AEveryThingGameState_Game, RemaningReadyTime);
	DOREPLIFETIME(AEveryThingGameState_Game, RemaningGameTime);
	DOREPLIFETIME(AEveryThingGameState_Game, RemaningBackToHouseTime);

	DOREPLIFETIME(AEveryThingGameState_Game, GameType);
	DOREPLIFETIME(AEveryThingGameState_Game, MapName);
	DOREPLIFETIME(AEveryThingGameState_Game, HouseName);
	DOREPLIFETIME(AEveryThingGameState_Game, bIsLANMatach);
	DOREPLIFETIME(AEveryThingGameState_Game, MaxPlayerNum);
	DOREPLIFETIME(AEveryThingGameState_Game, CurrentPlayerNum);
	DOREPLIFETIME(AEveryThingGameState_Game, ActualTeamNums);
	DOREPLIFETIME(AEveryThingGameState_Game, AllowedTeamNum);

	DOREPLIFETIME(AEveryThingGameState_Game, ChatPlayerState);	
}