// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInfoDiaplay.h"

#include "Online/EveryThingPlayerState_Game.h"
#include "Online/PlayerController_Game.h"
#include "Characters/GamePawn.h"
#include "Characters/GamePawnDurabilityComponent.h"
#include "Characters/GamePawnStaminaComponent.h"

void UPlayerInfoDiaplay::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	if (OwnerPC_G)
	{
		InitializeCurrentRoleName(OwnerPC_G->GetCurrentRoleName());
		OwnerPC_G->OnRoleNameUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnRoleNameUpdate);

		OnPlayerStateUpdate(OwnerPC_G->PlayerState);
		OwnerPC_G->OnPlayerStateUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnPlayerStateUpdate);
	}
}

void UPlayerInfoDiaplay::OnPlayerStateUpdate(class APlayerState* PlayerState)
{
	AEveryThingPlayerState_Game* OwnerETPS_G = Cast<AEveryThingPlayerState_Game>(PlayerState);
	if (OwnerETPS_G)
	{
		OnPlayerInfoUpdate(OwnerETPS_G->GetPlayerInfo());
		OnPlayerTeamUpdate(OwnerETPS_G->GetTeamID());
		OwnerETPS_G->OnTeamIDUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnPlayerTeamUpdate);
		OwnerETPS_G->OnPlayerInfoUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnPlayerInfoUpdate);
	}
}

void UPlayerInfoDiaplay::OnRoleNameUpdate(const FName& RoleName)
{
	InitializeCurrentRoleName(RoleName);

	APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(GetOwningPlayer());
	AGamePawn* OwnerGamePawn = OwnerPC_G ? Cast<AGamePawn>(OwnerPC_G->GetPawn()) : nullptr;
	if (OwnerGamePawn)
	{
		UGamePawnDurabilityComponent* OwnerDurabilityComp = OwnerGamePawn->GetDurabilityComp();
		UGamePawnStaminaComponent* OwnerStaminaComp = OwnerGamePawn->GetStaminaComp();

		checkf(OwnerDurabilityComp && OwnerStaminaComp, TEXT("-_- DurabilityComp and StaminaComp must be exists."));

		Durability = OwnerDurabilityComp->GetDurability();
		MaxDurability = OwnerDurabilityComp->GetMaxDurability();
		OwnerDurabilityComp->OnDurabilityUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnDurabilityUpdate);
		OwnerDurabilityComp->OnMaxDurabilityUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnMaxDurabilityUpdate);

		UpdateDurability(Durability, MaxDurability);

		Stamina = OwnerStaminaComp->GetStamina();
		MaxStamina = OwnerStaminaComp->GetMaxStamina();
		OwnerStaminaComp->OnStaminaUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnStaminaUpdate);
		OwnerStaminaComp->OnMaxStaminaUpdateDelegate.AddUObject(this, &UPlayerInfoDiaplay::OnMaxStaminaUpdate);

		UpdateStamina(Stamina, MaxStamina);
	}
}

void UPlayerInfoDiaplay::OnPlayerTeamUpdate(int32 TeamID)
{
	if (TeamID > 0)
	{
		UpdateTeamDisplay(true, TeamID);
	}
	else
	{
		UpdateTeamDisplay(false, -1);
	}
}