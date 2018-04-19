// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnStaminaComponent.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Characters/GamePawn.h"


UGamePawnStaminaComponent::UGamePawnStaminaComponent()
{
	OwnerGamePawn = Cast<AGamePawn>(GetOwner());

	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		checkf(OwnerGamePawn, TEXT("-_- Owner Game Pawn muse be exists."));
	}

	StaminaRecoverRateScale = 0.125f;
	StaminaRecoverRateDeltaSeconds = 0.2f;
}

void UGamePawnStaminaComponent::SetMaxStamina(float InMaxStamina)
{
	MaxStamina = InMaxStamina;
	OnMaxStaminaUpdate();

	UpdateStaminaRecoveRate();
}

//////////////////////////////////////////////////////////////////////////
/// Stamina
void UGamePawnStaminaComponent::ChangeStamina(float InStaminaOffset)
{
	Stamina -= InStaminaOffset;

	Stamina = FMath::Clamp<float>(Stamina, 0.f, MaxStamina);

	if (Stamina < MaxStamina)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (!TimerManager.TimerExists(StaminaAutoRecoverTimerHandle))
		{
			TimerManager.SetTimer(StaminaAutoRecoverTimerHandle, this, &UGamePawnStaminaComponent::StaminaAutoRecover, StaminaRecoverRateDeltaSeconds, true);
		}
	}

	OnStaminaUpdateDelegate.Broadcast(Stamina);
}

void UGamePawnStaminaComponent::SetStaminaRecoverRateScale(float InStaminaRecoverRateScale)
{
	StaminaRecoverRateScale = InStaminaRecoverRateScale;
	UpdateStaminaRecoveRate();
}

void UGamePawnStaminaComponent::UpdateStaminaRecoveRate()
{
	StaminaRecoverRate = MaxStamina * StaminaRecoverRateScale;
}

void UGamePawnStaminaComponent::StaminaAutoRecover()
{
	ChangeStamina(-StaminaRecoverRate * StaminaRecoverRateDeltaSeconds);

	if (Stamina == MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaAutoRecoverTimerHandle);
	}
}




void UGamePawnStaminaComponent::InitializeConsumeScale(float InCansumeScale, float InConsumeForceScale, float InConsumeTorqueScale, float InConsumeImpluseScale)
{
	CansumeScale = InCansumeScale;
	ConsumeForceScale = InConsumeForceScale;
	ConsumeTorqueScale = InConsumeTorqueScale;
	ConsumeImpluseScale = InConsumeImpluseScale;
}

//////////////////////////////////////////////////////////////////////////
/// On Use Force 
inline void UGamePawnStaminaComponent::OnConsumeForce(const FVector& Force) { ChangeStamina(GetConsumeForceValue(Force)); }
inline void UGamePawnStaminaComponent::OnConsumeTorqueInRadians(const FVector& Torque) { ChangeStamina(GetConsumeTorqueInRadiansValue(Torque)); }
inline void UGamePawnStaminaComponent::OnConsumeImpulse(const FVector& Impulse) { ChangeStamina(GetConsumeImpulseValue(Impulse)); }

inline bool UGamePawnStaminaComponent::CanConsumeForce(const FVector& Force) { return Stamina > GetConsumeForceValue(Force); }
inline bool UGamePawnStaminaComponent::CanConsumeTorqueInRadians(const FVector& Torque) { return Stamina > GetConsumeTorqueInRadiansValue(Torque); }
inline bool UGamePawnStaminaComponent::CanConsumeImpulse(const FVector& Impulse) { return Stamina > GetConsumeImpulseValue(Impulse); }

inline int32 UGamePawnStaminaComponent::GetConsumeForceValue(const FVector& Force) const { return Force.Size() * ConsumeForceScale * CansumeScale; }
inline int32 UGamePawnStaminaComponent::GetConsumeTorqueInRadiansValue(const FVector& Torque) const { return Torque.Size() * ConsumeTorqueScale * CansumeScale; }
inline int32 UGamePawnStaminaComponent::GetConsumeImpulseValue(const FVector& Impulse) const { return Impulse.Size() * ConsumeImpluseScale * CansumeScale; }

void UGamePawnStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGamePawnStaminaComponent, Stamina);
	DOREPLIFETIME(UGamePawnStaminaComponent, MaxStamina);
}