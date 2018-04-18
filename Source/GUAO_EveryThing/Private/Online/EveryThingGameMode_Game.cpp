// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_Game.h"

#include "EngineUtils.h"
#include "Engine/World.h"

#include "Online/PlayerController_Game.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameState_Game.h"
#include "Online/EveryThingGameSession.h"
#include "Online/EveryThingPlayerStart.h"
#include "UI/EveryThingHUD_Game.h"
#include "EveryThingGameInstance.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"

#include "PlayerFootballPawn.h"

AEveryThingGameMode_Game::AEveryThingGameMode_Game()
{
	NormalDamageScale = 1.f;
	SpecificToDamageScale = 1.5f;
	bStartPlayersAsSpectators = true;
	bPauseable = false;
	bUseSeamlessTravel = true;
	bBackToHome = false;

	DefaultPawnClass = APlayerFootballPawn::StaticClass();
	GameSessionClass = AEveryThingGameSession::StaticClass();
	PlayerControllerClass = APlayerController_Game::StaticClass();
	PlayerStateClass = AEveryThingPlayerState_Game::StaticClass();
	GameStateClass = AEveryThingGameState_Game::StaticClass();
	HUDClass = AEveryThingHUD_Game::StaticClass();

	ETPlayerStatrClass = AEveryThingPlayerStart::StaticClass();
}


//////////////////////////////////////////////////////////////////////////
/// Game flow path

void AEveryThingGameMode_Game::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AEveryThingGameMode_Game::InitGameState()
{
	Super::InitGameState();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Sessions = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	FOnlineSessionSettings* OwnerOnlineSessionSetting = Sessions.IsValid() ? Sessions->GetSessionSettings(NAME_GameSession) : nullptr;
	FNamedOnlineSession* OwnerNamedSession = Sessions->GetNamedSession(NAME_GameSession);
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerOnlineSessionSetting && OwnerETGS_G && OwnerNamedSession)
	{		
		FString HouseName, GameType, MapName;
		OwnerOnlineSessionSetting->Get<FString>(SETTING_GAMEMODE, GameType);
		OwnerOnlineSessionSetting->Get<FString>(SETTING_MAPNAME, MapName);
		OwnerOnlineSessionSetting->Get<FString>(FName("HouseName"), HouseName);
		
		int32 MaxPlayersNum = OwnerOnlineSessionSetting->NumPublicConnections;
		int32 CurrentPlayerNum = MaxPlayersNum - OwnerNamedSession->NumOpenPublicConnections;
		
		UE_LOG(LogTemp, Log, TEXT("-_- all palyer: %d"), CurrentPlayerNum);

		OwnerETGS_G->InitializeETGameState(GameType, MapName, HouseName, OwnerOnlineSessionSetting->bIsLANMatch, MaxPlayersNum, CurrentPlayerNum);
	}
}

void AEveryThingGameMode_Game::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	HandlePlayerLogin(NewPlayer);
}


void AEveryThingGameMode_Game::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);

	HandlePlayerLogin(NewPC);
}


void AEveryThingGameMode_Game::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(NewPlayer);
	if (OwnerPC_G)
	{
		OwnerPC_G->ClientStartPlayer();
	}
}

AActor* AEveryThingGameMode_Game::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	if (GetWorld() && ETPlayerStatrClass)
	{
		for (TActorIterator<AEveryThingPlayerStart> It(GetWorld(), ETPlayerStatrClass); It; ++It)
		{
			if (It->CanSpawnGamePawn(Player))
			{
				return *It;
			}
		}
	}

	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}

UClass* AEveryThingGameMode_Game::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	APlayerController_Game* InPC_G = Cast<APlayerController_Game>(InController);
	AEveryThingPlayerState_Game* InETPS_G = InPC_G ? Cast<AEveryThingPlayerState_Game>(InPC_G->PlayerState) : nullptr;
	if (InETPS_G)
	{
		const FName& RoleName = InETPS_G->GetLastSelectRoleName();
		if (!RoleName.IsNone())
		{
			return UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetRoleClassFromName(RoleName).Get();
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AEveryThingGameMode_Game::Logout(AController* Exiting)
{
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->OnPlayerLogout(Exiting);
	}

	Super::Logout(Exiting);
}

//////////////////////////////////////////////////////////////////////////
/// Handle
void AEveryThingGameMode_Game::HandlePlayerLogin(APlayerController* NewPlayer)
{
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->OnPlayerPostLogin(NewPlayer);

		if (OwnerETGS_G->IsETGameStarted())
		{
			RestartPlayer(NewPlayer);
		}
		else
		{
			APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(NewPlayer);
			if (OwnerPC_G)
			{
				switch (OwnerETGS_G->GetETGameState())
				{
					case EETGameState::WaitForHousePlayerLoad:
						OwnerPC_G->ClientWaitForHousePlayerLoad();
						break;
					case EETGameState::ReadyToStart:
						OwnerPC_G->ClientReadyToStart();
						break;
				}
			}
		}
	}
}
void AEveryThingGameMode_Game::HandleETGameReay()
{
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->StartReadyCountDown();
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(It->Get());
		if (OwnerPC_G) { OwnerPC_G->ClientReadyToStart(); }
	}
}
void AEveryThingGameMode_Game::HandleETGameStart()
{
	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->StartGameTimeCountDown();
	}
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		RestartPlayer(It->Get());
	}
}
bool AEveryThingGameMode_Game::HandleETGameIsOver()
{
	return true;
}
void AEveryThingGameMode_Game::HandleETGameOver()
{

	AEveryThingGameState_Game* OwnerETGS_G = GetGameState<AEveryThingGameState_Game>();
	if (OwnerETGS_G)
	{
		OwnerETGS_G->StartBackToHouseTimeCountDown();
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController_Game* CurrentPC_G = Cast<APlayerController_Game>(It->Get());
		AEveryThingPlayerState_Game* CurrentETPS_G = Cast<AEveryThingPlayerState_Game>(CurrentPC_G->PlayerState);
		if (CurrentPC_G && CurrentETPS_G)
		{
			CurrentPC_G->ClientGameOver(CurrentETPS_G->GetGameScore());
		}
	}
}
void AEveryThingGameMode_Game::HandlerETBackToHouse()
{
	UWorld* World = GetWorld();
	UEveryThingGameInstance* OwnetETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
	if (World && OwnetETGI)
	{
		bBackToHome = true;
		World->ServerTravel(FString::Printf(TEXT("%s?listen"), *OwnetETGI->HouseLevelName.ToString()));
	}
}



//////////////////////////////////////////////////////////////////////////
/// Damage


bool AEveryThingGameMode_Game::CanTakeDamage(AGamePawn* PlayerPawn, AActor* OtherActor)
{
	return true;
}

float AEveryThingGameMode_Game::GetDamageFromHitableHit(AGamePawn* PlayerPawn, const FVector& NormalInpulse, const FHitResult& Hit) const
{
	float Damage = 0.f;
	AActor* OtherActor = Hit.GetActor();
	if (PlayerPawn && OtherActor)
	{
		float Speed = FVector::DotProduct(PlayerPawn->GetVelocity(), Hit.Normal);
		if (Speed > 200.f)
		{
			Damage = NormalInpulse.Size() / 10000.f;
			FString name = OtherActor->GetName();
			UE_LOG(LogTemp, Log, TEXT("Hit! speed %f,impulse %f,name %s"), Speed, NormalInpulse.Size(), *name);
		}
	}
	return Damage;
}

float AEveryThingGameMode_Game::GetDamageFromActorHit(AGamePawn* PlayerPawn, const FVector& NormalInpulse, const FHitResult& Hit) const
{
	float Damage = 0.f;
	AActor* OtherActor = Hit.GetActor();
	if (PlayerPawn && OtherActor)
	{
		float Speed = FVector::DotProduct(PlayerPawn->GetVelocity(), Hit.Normal);
		float UpSpeed = FMath::Clamp(PlayerPawn->GetVelocity().Z,0.f,1000.f);
		if (Speed > 200.f)
		{	
			if (UpSpeed <= 300.f || UpSpeed >= 800.f)
			{
				float MyStability = 100.f;
				Damage = Speed*(NormalInpulse.Size() / MyStability) / 40000.f;
				FString name = OtherActor->GetName();
			}
			else 
			{
				float FreeFallModifier = 2.0f;
				float MyStability = 100.f;
				Damage = (Speed + UpSpeed * FreeFallModifier)*(NormalInpulse.Size() / MyStability) / 40000.f;
				FString name = OtherActor->GetName();
			}
		}
		
	}
	return Damage;
}

float AEveryThingGameMode_Game::GetDamageFromGamePawnHit(AGamePawn* PlayerPawn, AGamePawn* OtherPawn, const FVector& NormalInpulse, const FHitResult& Hit) const
{
	float Damage = 0.f;
	if (PlayerPawn && OtherPawn)
	{
		float Speed = FVector::DotProduct(OtherPawn->GetVelocity(), Hit.Normal);
		if (Speed > 100.f)
		{
			float OtherStability = 100.f;//100-200
			float MyAgility = 100.f;//100-200
			float OtherAgility = 100.f;//100-200
			Damage = (Speed / OtherStability) * (1 + (MyAgility - OtherAgility) / (MyAgility + OtherAgility))*NormalInpulse.Size() / 100000.f;
		}
	}
	return Damage;
}

float AEveryThingGameMode_Game::GetActualDamage(float InDamage, EElementType InDamageType, EElementType PawnElementType, float ElementResistance) const
{
	return InDamage * GetDamageScaleFromElementType(InDamageType, PawnElementType);
}

float AEveryThingGameMode_Game::GetDamageScaleFromElementType(EElementType CauserElementType, EElementType AcceptElementType) const
{
	switch (CauserElementType)
	{
		case EElementType::Fire:
			if (AcceptElementType == EElementType::Water) { return SpecificToDamageScale; }
			break;
		case EElementType::Water:
			if (AcceptElementType == EElementType::Fire) { return SpecificToDamageScale; }
			break;
	}

	return NormalDamageScale;
}

void AEveryThingGameMode_Game::BeginDestroy()
{
	if (!bBackToHome)
	{
		AEveryThingGameSession* OwnerGameSession = Cast<AEveryThingGameSession>(GameSession);
		if (OwnerGameSession) { OwnerGameSession->DestroySession(); }
	}

	Super::BeginDestroy();
}