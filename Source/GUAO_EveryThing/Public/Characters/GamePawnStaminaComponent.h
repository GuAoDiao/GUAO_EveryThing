// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePawnStaminaComponent.generated.h"

class AGamePawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UGamePawnStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGamePawnStaminaComponent();
	
protected:
	AGamePawn* OwnerGamePawn;

	//////////////////////////////////////////////////////////////////////////
	/// Stamina
public:
	UFUNCTION(BlueprintPure)
	float GetStamina() const { return Stamina; }
	UFUNCTION(BlueprintPure)
	float GetMaxStamina() const { return MaxStamina; }

	void ChangeStamina(float InStaminaOffset);

	void SetStamina(float InStamina) { Stamina = InStamina; OnStaminaUpdate(); }
	void SetMaxStamina(float InMaxStamina);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaUpdateDelegate, float /* Stamina */);
	FOnStaminaUpdateDelegate OnStaminaUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaUpdateDelegate, float /* MaxStamina */);
	FOnStaminaUpdateDelegate OnMaxStaminaUpdateDelegate;

protected:
	UFUNCTION()
	void OnRep_Stamina() { OnStaminaUpdate(); }
	void OnStaminaUpdate() { OnStaminaUpdateDelegate.Broadcast(Stamina); }
	UFUNCTION()
	void OnRep_MaxStamina() { OnMaxStaminaUpdate(); }
	void OnMaxStaminaUpdate() { OnMaxStaminaUpdateDelegate.Broadcast(MaxStamina); }

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina)
	float Stamina;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	float MaxStamina;

	//////////////////////////////////////////////////////////////////////////
	/// Stamina Auto Recover
public:
	float GetStaminaRecoverRateScale() const { return StaminaRecoverRateScale; }
	void SetStaminaRecoverRateScale(float InStaminaRecoverRateScale);

protected:
	void UpdateStaminaRecoveRate();
	void StaminaAutoRecover();

	UPROPERTY(BlueprintReadOnly)
	float StaminaRecoverRate;
	UPROPERTY(BlueprintReadOnly)
	float StaminaRecoverRateScale;
	UPROPERTY(BlueprintReadOnly)
	float StaminaRecoverRateDeltaSeconds;

	FTimerHandle StaminaAutoRecoverTimerHandle;

	//////////////////////////////////////////////////////////////////////////
	/// On Use Force 
public:
	void InitializeConsumeScale(float InCansumeScale, float InConsumeForceScale, float InConsumeTorqueScale, float InConsumeImpluseScale);

	void ChangeCansumeScale(int32 InCansumeScale) { CansumeScale = InCansumeScale; }
	int32 GetCansumeScale() const { return CansumeScale; }

	void OnConsumeForce(const FVector& Force);
	void OnConsumeTorqueInRadians(const FVector& Torque);
	void OnConsumeImpulse(const FVector& Impulse);

	bool CanConsumeForce(const FVector& Force);
	bool CanConsumeTorqueInRadians(const FVector& Torque);
	bool CanConsumeImpulse(const FVector& Impulse);

protected:
	int32 GetConsumeForceValue(const FVector& Force) const;
	int32 GetConsumeTorqueInRadiansValue(const FVector& Torque) const;
	int32 GetConsumeImpulseValue(const FVector& Impulse) const;

	float CansumeScale;
	float ConsumeForceScale;
	float ConsumeTorqueScale;
	float ConsumeImpluseScale;
};
