// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController_Game.h"

#include "UnrealNetwork.h"
#include "TimerManager.h"
#include "GameFramework/InputSettings.h"

#include "EveryThingTypes.h"
#include "EveryThingGameInstance.h"
#include "UI/EveryThingHUD_Game.h"
#include "EveryThingAssetManager.h"
#include "ChatWindow/ChatComponent.h"
#include "Characters/GamePawn.h"
#include "Characters/PlayerPawnInterface.h"
#include "Characters/PlayerPawnComponent.h"
#include "Characters/Moves/Moves.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "Characters/Movement/Components/GamePawnMovementComponent.h"
#include "SceneObject/HitAbleInterface.h"
#include "Online/EveryThingPlayerState_Game.h"
#include "Online/EveryThingGameMode_Game.h"
#include "Characters/GamePawnManager.h"


#define LOCTEXT_NAMESPACE "Everything_Online_PlayerController_Game"

APlayerController_Game::APlayerController_Game()
{
	ChatComponent = CreateDefaultSubobject<UChatComponent>(TEXT("ChatComponent"));

	CurrentRoleName = NAME_None;
}

void APlayerController_Game::BeginPlay()
{
	Super::BeginPlay();

	// set all role names
	const TMap<FName, FRoleInfo>& AllRolesInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetAllRolesInfo();
	for (TMap<FName, FRoleInfo>::TConstIterator It(AllRolesInfo); It; ++It)
	{
		AllRoleNames.AddUnique(It.Key());
	}

	SetInputMode(FInputModeGameOnly());
}

void APlayerController_Game::RebindInput()
{
	if (!InputComponent) { return; }

	// Clear all bindings;
	InputComponent->ClearActionBindings();
	InputComponent->AxisBindings.Empty();
	ResetAxisAndActionMapping();

	InputComponent->BindAction("DisplayGameMenu", IE_Pressed, this, &APlayerController_Game::DisplayGameMenu);
	InputComponent->BindAction("FocusToChatWindow", IE_Pressed, this, &APlayerController_Game::FocusToChatWindow);
	
	InputComponent->BindAction("ToggleScoreBoard", IE_Pressed, this, &APlayerController_Game::DisplayScoreBoard);
	InputComponent->BindAction("ToggleScoreBoard", IE_Released, this, &APlayerController_Game::RemoveScoreBoard);


	InputComponent->BindAxis("Turn", this, &APlayerController_Game::Turn);
	InputComponent->BindAxis("LookUp", this, &APlayerController_Game::LookUp);

	InputComponent->BindAction("TogglePawn", IE_Pressed, this, &APlayerController_Game::StartToggleRole);
	InputComponent->BindAction("TogglePawn", IE_Released, this, &APlayerController_Game::StopToggleRole);

	if (OwnerGamePawn)
	{
		ToggleToNewAttackComponent(OwnerGamePawn->GetAttackComponent());
		ToggleToNewSkillComponent(OwnerGamePawn->GetSkillComponent());
		
		UGamePawnMovementComponent* OwnerPlayerMovementComp = OwnerGamePawn->GetGamePawnMovementComponent();
		if (OwnerPlayerMovementComp) { OwnerPlayerMovementComp->RebindInputComp(InputComponent); }
	}

	if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->RebindInputComp(InputComponent); }
}

void APlayerController_Game::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OwnerGamePawn = Cast<AGamePawn>(InPawn);
	IPlayerPawnInterface* OwnerPlayerPawn = Cast<IPlayerPawnInterface>(InPawn);
	OwnerPlayerPawnComp = OwnerPlayerPawn ? OwnerPlayerPawn->GetPlayerPawnComponent() : nullptr;

	// try to get actual role name when first SetPawn.
	if (InPawn == nullptr) { CurrentRoleName = NAME_None; }
	else if (CurrentRoleName.IsNone() && OwnerGamePawn)
	{
		UClass* CurrentPawnClass = OwnerGamePawn->GetClass();
		CurrentRoleName = CurrentPawnClass ? UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetRoleNameFromClass(CurrentPawnClass) : NAME_None;
		OnCurrentRoleNameUpdate();
	}

	RebindInput();
}


void APlayerController_Game::ResetAxisAndActionMapping()
{
	UInputSettings* InpueSettings = UInputSettings::GetInputSettings();
	if (!InpueSettings) { return; }

	UDataTable* InputActionDatable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("InputAction"));
	if (InputActionDatable)
	{
		TArray<FInputAction*> ActionInfoInDatatable;
		InputActionDatable->GetAllRows<FInputAction>(TEXT("found all input axis mapping"), ActionInfoInDatatable);


		for (FInputAction* ActionInfo : ActionInfoInDatatable)
		{
			TArray<FInputActionKeyMapping> AllActionKeyMappings;
			InpueSettings->GetActionMappingByName(ActionInfo->ActionName, AllActionKeyMappings);
			for (const FInputActionKeyMapping& ActionKeyMapping : AllActionKeyMappings)
			{
				InpueSettings->RemoveActionMapping(ActionKeyMapping);
			}
		}

		for (FInputAction* ActionInfo : ActionInfoInDatatable)
		{
			InpueSettings->AddActionMapping(FInputActionKeyMapping(ActionInfo->ActionName, FKey(ActionInfo->Input), ActionInfo->bInShift, ActionInfo->bInCtrl, ActionInfo->bInAlt, ActionInfo->bInCmd));
		}
	}

	UDataTable* InputAxisDatable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("InputAxis"));
	if (InputAxisDatable)
	{
		TArray<FInputAxis*> AxisInfoInDatatable;
		InputAxisDatable->GetAllRows<FInputAxis>(TEXT("found all input axis mapping"), AxisInfoInDatatable);


		for (FInputAxis* AxisInfo : AxisInfoInDatatable)
		{
			TArray<FInputAxisKeyMapping> AllAxisKeyMappings;
			InpueSettings->GetAxisMappingByName(AxisInfo->AxisName, AllAxisKeyMappings);
			for (const FInputAxisKeyMapping& AxisKeyMapping : AllAxisKeyMappings)
			{
				InpueSettings->RemoveAxisMapping(AxisKeyMapping);
			}
		}

		for (FInputAxis* AxisInfo : AxisInfoInDatatable)
		{
			InpueSettings->AddAxisMapping(FInputAxisKeyMapping(AxisInfo->AxisName, FKey(AxisInfo->Input), AxisInfo->AxisVlaue));
		}
	}
}

void APlayerController_Game::RemoveActionAndAxisBindings(const TArray<FName>& BindingsName)
{
	if (!InputComponent) { return; }

	// clear all axis bindings
	for (int32 i = 0, l = InputComponent->AxisBindings.Num(); i < l; ++i)
	{
		if (BindingsName.Contains(InputComponent->AxisBindings[i].AxisName))
		{
			InputComponent->AxisBindings.RemoveAt(i);
			--i; --l;
		}
	}

	// clear all action bindings
	for (int32 i = 0, l = InputComponent->GetNumActionBindings(); i < l; ++i)
	{
		if (BindingsName.Contains(InputComponent->GetActionBinding(i).ActionName))
		{
			InputComponent->RemoveActionBinding(i);
			--i; --l;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// Game flow path

void APlayerController_Game::ClientWaitForHousePlayerLoad_Implementation()
{
	AEveryThingHUD_Game* OwnerETHUD_G = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETHUD_G) { OwnerETHUD_G->ToggleToTargetGameUIState(EETGameState::WaitForHousePlayerLoad); }
}

void APlayerController_Game::ClientReadyToStart_Implementation()
{
	AEveryThingHUD_Game* OwnerETHUD_G = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETHUD_G) { OwnerETHUD_G->ToggleToTargetGameUIState(EETGameState::ReadyToStart); }
}

void APlayerController_Game::ClientStartPlayer_Implementation()
{
	AEveryThingHUD_Game* OwnerETHUD_G = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETHUD_G) { OwnerETHUD_G->ToggleToTargetGameUIState(EETGameState::Gameing); }
}

void APlayerController_Game::ClientGameOver_Implementation(int32 GetGold)
{
	UEveryThingGameInstance* OwnerETGI = Cast<UEveryThingGameInstance>(GetGameInstance());
	if (OwnerETGI)
	{
		OwnerETGI->AddGold(GetGold);
	}

	AEveryThingHUD_Game* OwnerETHUD_G = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETHUD_G)
	{
		OwnerETHUD_G->ToggleToTargetGameUIState(EETGameState::GameOver);
		OwnerETHUD_G->ShowGameOver(GetGold);
	}
}


//////////////////////////////////////////////////////////////////////////
/// All Game State Info
void APlayerController_Game::ClientOnAcceptCure_Implementation(const FString& CauserName, float Treatment) { OnAcceptCureDelegate.Broadcast(CauserName, Treatment); }
void APlayerController_Game::ClientOnAcceptDamage_Implementation(const FString& CauserName, float Damage) { OnAcceptDamageDelegate.Broadcast(CauserName, Damage); }
void APlayerController_Game::ClientOnAcceptCriticalDamage_Implementation(const FString& CauserName) { OnAcceptCriticalDamageDelegate.Broadcast(CauserName); }


void APlayerController_Game::ClientOnTakeCure_Implementation(const FString& AccepterName, float Treatment) { OnTakeCureDelegate.Broadcast(AccepterName, Treatment); }
void APlayerController_Game::ClientOnTakeDamage_Implementation(const FString& AccepterName, float Damage) { OnTakeDamageDelegate.Broadcast(AccepterName, Damage); }
void APlayerController_Game::ClientOnTakeCriticalDamage_Implementation(const FString& AccepterName) { OnTakeCriticalDamageDelegate.Broadcast(AccepterName); }


void APlayerController_Game::ClientOnBeKilled_Implementation(const FString& KillerName) { OnBeKilledDelegate.Broadcast(KillerName); }
void APlayerController_Game::ClientOnSuicided_Implementation() { OnSuicidedDelegate.Broadcast(); }
void APlayerController_Game::ClientOnKillOther_Implementation(const FString& KilledName) { OnKillOtherDelegate.Broadcast(KilledName); }

void APlayerController_Game::ClientOnGamePawnBeKilled_Implementation(const FString& KilledName, const FString& KillerName) { OnGamePawnBeKilledDelegate.Broadcast(KilledName, KillerName); }
void APlayerController_Game::ClientOnGamePawnSuicided_Implementation(const FString& KilledName) { OnGamePawnSuicidedDelegate.Broadcast(KilledName); }

void APlayerController_Game::ClientCreatePlayerFightInfo_Implementation(const FText& FightInfo)
{
	AEveryThingHUD_Game* OwnerETGH = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETGH) { OwnerETGH->ClientCreatePlayerFightInfo(FightInfo); }
}

//////////////////////////////////////////////////////////////////////////
/// Player State
void APlayerController_Game::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateUpdateDelegate.Broadcast(PlayerState);
}


//////////////////////////////////////////////////////////////////////////
/// For IGamePawnControllerInterface
AActor* APlayerController_Game::GetAttackTarget() { return OwnerPlayerPawnComp ? OwnerPlayerPawnComp->GetAttackTarget() : nullptr; }

//////////////////////////////////////////////////////////////////////////
/// Visual Angle
void APlayerController_Game::Turn(float AxisValue) { if (AxisValue != 0.f && OwnerPlayerPawnComp) { OwnerPlayerPawnComp->Turn(AxisValue); } }
void APlayerController_Game::LookUp(float AxisValue) { if (AxisValue != 0.f && OwnerPlayerPawnComp) { OwnerPlayerPawnComp->LookUp(AxisValue); } }


//////////////////////////////////////////////////////////////////////////
/// UI
void APlayerController_Game::DisplayGameMenu()
{
	AEveryThingHUD_Game* OwnerETGH = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETGH) { OwnerETGH->DisplayGameMenu(); }
}

void APlayerController_Game::FocusToChatWindow()
{
	AEveryThingHUD_Game* OwnerETGH = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETGH) { OwnerETGH->FocusToChatWindow(); }
}

void APlayerController_Game::DisplayScoreBoard()
{
	AEveryThingHUD_Game* OwnerETGH = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETGH) { OwnerETGH->DisplayScoreBoard(); }
}
void APlayerController_Game::RemoveScoreBoard()
{
	AEveryThingHUD_Game* OwnerETGH = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETGH) { OwnerETGH->RemoveScoreBoard(); }
}

//////////////////////////////////////////////////////////////////////////
/// Attack

void APlayerController_Game::ToggleToNewAttackComponent(UAttackComponent* InAttackComp)
{
	RemoveActionAndAxisBindings(TArray<FName>{"CommonAttack", "SpecialAttack"});

	if (InAttackComp)
	{
		FGamePawnMoves* CommonAttack = InAttackComp->GetCommantAttackSkilledness();
		if (CommonAttack) { CommonAttack->RebindInput(InputComponent); }
		FGamePawnMoves* SpecialAttack = InAttackComp->GetSpecialAttackSkilledness();
		if (SpecialAttack) { SpecialAttack->RebindInput(InputComponent); }
	}
}

//////////////////////////////////////////////////////////////////////////
/// Skill
void APlayerController_Game::ToggleToNewSkillComponent(USkillComponent* InSkillComp)
{
	RemoveActionAndAxisBindings(TArray<FName>{"FirstSkill", "SecondSkill", "UltimateSkill"});

	if (InSkillComp)
	{
		FGamePawnMoves* FirstSkill = InSkillComp->GetFirstSkillSkilledness();
		if (FirstSkill) { FirstSkill->RebindInput(InputComponent); }

		FGamePawnMoves* SecondSkill = InSkillComp->GetSecondSkillSkilledness();
		if (SecondSkill) { SecondSkill->RebindInput(InputComponent); }

		FGamePawnMoves* UltimateSkill = InSkillComp->GetUltimateSkillSkilledness();
		if (UltimateSkill) { UltimateSkill->RebindInput(InputComponent); }
	}
}



//////////////////////////////////////////////////////////////////////////
/// Toggle Role
void APlayerController_Game::StartToggleRole()
{
	AEveryThingHUD_Game* OwnerETGH = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETGH) { OwnerETGH->ToggleSelectRolesBox(true); }

	if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->GetOnPressNumberKeyboardDelegate().BindUObject(this, &APlayerController_Game::ToggleRoleWithIndex); }
}
void APlayerController_Game::StopToggleRole()
{
	AEveryThingHUD_Game* OwnerETGH = Cast<AEveryThingHUD_Game>(GetHUD());
	if (OwnerETGH) { OwnerETGH->ToggleSelectRolesBox(false); }

	if (OwnerPlayerPawnComp) { OwnerPlayerPawnComp->GetOnPressNumberKeyboardDelegate().Unbind(); }
}

void APlayerController_Game::ToggleRoleWithIndex(int32 NumberIndex)
{
	if (AllRoleNames.IsValidIndex(NumberIndex)) { ToggleRoleWithName(AllRoleNames[NumberIndex]); }
}

void APlayerController_Game::ToggleRoleWithName(const FName& TargetRoleName)
{
	// Get owning EverythingPlayerState and the world is exists
	AEveryThingPlayerState_Game* OwnerETPS = Cast<AEveryThingPlayerState_Game>(PlayerState);
	if (!OwnerETPS)
	{
		ClientCreatePlayerFightInfo(LOCTEXT("ToggleRoleWhenNotFonutPlayer", "Can't find Owner Player State."));
		return;
	}
	
	// try to find Target Role Name is exists
	const FPlayerInfo& PlayerInfo = OwnerETPS->GetPlayerInfo();
	if (!PlayerInfo.AllHaveRoleNames.Contains(TargetRoleName))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("TargetRoleName"), FText::FromName(TargetRoleName));
		ClientCreatePlayerFightInfo(FText::Format(LOCTEXT("ToggleRoleWhenNotHaveTargetRole", "Don't have Target role name : {TargetRoleName}."), Arguments));
		return;
	}
	
	// Check target pawn class is exists
	UClass* TargetPawnClass = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetRoleClassFromName(TargetRoleName).Get();
	if (!TargetPawnClass)
	{
		ClientCreatePlayerFightInfo(LOCTEXT("ToggleRoleWhenNotFoundTargetRoleClass", "Can't find Target Role Class."));
		return;
	}

	// check target pawn and current pawn isn't Parent-child
	if (CurrentRoleName == TargetRoleName)
	{
		ClientCreatePlayerFightInfo(LOCTEXT("ToggleRoleWhenSameRole", "Already is the target Role, needn't to toggle."));
		return;
	}

	// after code only run on server
	if (!HasAuthority())
	{
		ServerToggleRole(TargetRoleName);
		return;
	}


	// Hide current Pawn and disable it's collision
	APawn* OwnerPawn = GetPawn();
	
	checkf(OwnerPawn, TEXT("Can't find old pawn when toggle pawn"));

	OwnerPawn->SetActorEnableCollision(false);
	OwnerPawn->SetActorHiddenInGame(true);

	// spawn and possess new pawn
	FVector Location = OwnerPawn->GetActorLocation();
	Location.Z += 100.f;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AGamePawn* NewPawn = GetWorld()->SpawnActor<AGamePawn>(TargetPawnClass, Location, OwnerPawn->GetActorForwardVector().Rotation());
	checkf(NewPawn, TEXT("Can't create new pawn when toggle pawn"));
	Possess(NewPawn);


	// destroy old pawn
	OwnerPawn->Destroy();

	// update current pawn class
	CurrentRoleName = TargetRoleName;
	OnCurrentRoleNameUpdate();

	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("TargetRoleName"), FText::FromName(CurrentRoleName));
	ClientCreatePlayerFightInfo(FText::Format(LOCTEXT("ToggleRoleWhenSuccess", "Success toggle toggle to target role : {TargetRoleName}."), Arguments));
	
	StopToggleRole();
}

bool APlayerController_Game::ServerToggleRole_Validate(const FName& TargetRoleName) { return true; }
void APlayerController_Game::ServerToggleRole_Implementation(const FName& TargetRoleName)
{
	ToggleRoleWithName(TargetRoleName);
}


void APlayerController_Game::DelayToRestartGamePawn(float DelayTime)
{
	FTimerHandle DelayToRestartTimer;
	GetWorldTimerManager().SetTimer(DelayToRestartTimer, this, &APlayerController_Game::RestartGamePawn, DelayTime, false);
}

void APlayerController_Game::RestartGamePawn()
{
	AEveryThingGameMode_Game* OwnerETGM_G = GetWorld() ? GetWorld()->GetAuthGameMode<AEveryThingGameMode_Game>() : nullptr;
	if (OwnerETGM_G)
	{
		OwnerETGM_G->RestartPlayer(this);
	}
}


void APlayerController_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerController_Game, CurrentRoleName);
}



#undef LOCTEXT_NAMESPACE