// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnDurabilityComponent.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"

#include "Characters/GamePawn.h"
#include "Online/EverythingGameState_Game.h"

UGamePawnDurabilityComponent::UGamePawnDurabilityComponent()
{
	OwnerGamePawn = Cast<AGamePawn>(GetOwner());

	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		checkf(OwnerGamePawn, TEXT("-_- Owner Game Pawn muse be exists."));
	}
}

//////////////////////////////////////////////////////////////////////////
/// Cure, Damage, Death

void UGamePawnDurabilityComponent::Healed(AActor* Curer, float HealingValue)
{
	ChangeDurability(HealingValue);

	AEveryThingGameState_Game* OwnerETGS_G = Cast<AEveryThingGameState_Game>(GetWorld()->GetGameState());
	if (OwnerETGS_G) { OwnerETGS_G->OnGamePawnAcceptCure(OwnerGamePawn, Curer, HealingValue); }
}
void UGamePawnDurabilityComponent::ChangeDurability(float DurabilityOffset)
{
	Durability += DurabilityOffset;
	if (Durability < 0.f) { Durability = 0.f; }
	if (Durability > MaxDurability) { Durability = MaxDurability; }

	OnDurabilityUpdateDelegate.Broadcast(Durability);
}

float UGamePawnDurabilityComponent::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = DamageAmount;

	ChangeDurability(-FinalDamage);

	LastDamageCauserActor = DamageCauser;

	AEveryThingGameState_Game* OwnerETGS_G = Cast<AEveryThingGameState_Game>(GetWorld()->GetGameState());
	if (OwnerETGS_G) { OwnerETGS_G->OnGamePawnAcceptDamage(OwnerGamePawn, DamageCauser, FinalDamage); }

	if (Durability == 0.f)
	{
		if (OwnerETGS_G) { OwnerETGS_G->OnGamePawnAcceptCriticalDamage(OwnerGamePawn, DamageCauser); }

		OwnerGamePawn->GamePawnDeath();
	}

	return FinalDamage;
}



void UGamePawnDurabilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGamePawnDurabilityComponent, Durability);
	DOREPLIFETIME(UGamePawnDurabilityComponent, MaxDurability);
}