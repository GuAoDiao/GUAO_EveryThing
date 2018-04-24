// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePawnDurabilityComponent.generated.h"

class AGamePawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UGamePawnDurabilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGamePawnDurabilityComponent();
protected:
	AGamePawn* OwnerGamePawn;

	//////////////////////////////////////////////////////////////////////////
	/// Cure, Damage
public:
	AActor* GetLastDamageCauser() const { return LastDamageCauserActor; }
public:
	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	void Healed(AActor* Curer, float Treatment);
protected:
	UPROPERTY()
	AActor* LastDamageCauserActor;
	

		
	
	//////////////////////////////////////////////////////////////////////////
	/// Durability
public:
	UFUNCTION(BlueprintPure)
	float GetDurability() const { return Durability; }
	UFUNCTION(BlueprintPure)
	float GetMaxDurability() const { return MaxDurability; }

	void SetDurability(int32 InDurability) { Durability = InDurability; OnDurabilityUpdate(); }
	void SetMaxDurability(int32 InMaxDurability) { MaxDurability = InMaxDurability; OnMaxDurabilityUpdate(); }

	void ChangeDurability(float DurabilityOffset);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDurabilityUpdateDelegate, float /* Durability */);
	FOnDurabilityUpdateDelegate OnDurabilityUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxDurabilityUpdateDelegate, float /* MaxDurability */);
	FOnMaxDurabilityUpdateDelegate OnMaxDurabilityUpdateDelegate;
protected:

	UFUNCTION()
	void OnRep_Durability() { OnDurabilityUpdate(); }
	void OnDurabilityUpdate() { OnDurabilityUpdateDelegate.Broadcast(Durability); }

	UFUNCTION()
	void OnRep_MaxDurability() { OnMaxDurabilityUpdate(); }
	void OnMaxDurabilityUpdate() { OnMaxDurabilityUpdateDelegate.Broadcast(MaxDurability); }

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Durability)
	float Durability;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxDurability)
	float MaxDurability;	
};
