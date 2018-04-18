// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePawnMovementComponent.h"
#include "RotaryMovementComponent.generated.h"

class IHitAbleInterface;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GUAO_EVERYTHING_API URotaryMovementComponent : public UGamePawnMovementComponent
{
	GENERATED_BODY()

public:
	URotaryMovementComponent();
	
	virtual void BeginDestroy() override;

	virtual void RebindInputComp(class UInputComponent* OwnerInputComp);

	//////////////////////////////////////////////////////////////////////////
	/// Move Actual 
	FTimerHandle MoveTimer;
	void StartMoveTimer();
	void CloseMoveTimer();
	void PauseMoveTimer();
	void UnpauseMoveTimer();
	void MoveTimerImplementation();


	virtual void UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale = 1.f) override;
public:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Move(const FVector& Direction, float AxisValue);
	void MoveToLocation(const FVector& Location, float AxisValue);
	void AcceptForceImpulse(const FVector& Location, const FVector& Force);

	void StartJump();

	
public:
	void SetIsJumping(bool bInIsJumping) { bIsJumping = bInIsJumping; }
	bool CanJump() { return !bIsJumping; }

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMove(const FVector& Direction, float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAcceptForceImpulse(const FVector& Location, const FVector& Force);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartJump();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float SpeedForceBase;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FastSpeedScale;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float JumpForceBase;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FastJumpScale;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient, Replicated)
	float CurrentSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient, Replicated)
	float CurrentJumpForce;

	//////////////////////////////////////////////////////////////////////////
	/// Movement State
public:
	void ToogleMovementState();

protected:
	void SetMovementState(bool bIsFastMovementState);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToogleMovementState();
	UFUNCTION()
	void OnRep_IsFastMovementState() { SetMovementState(bIsFastMovementState); }

	UPROPERTY(ReplicatedUsing = OnRep_IsFastMovementState, Transient)
	bool bIsFastMovementState;


private:
	UPROPERTY(Replicated, Transient)
	bool bIsJumping;

	bool bWantsToMove;
	FVector WantMoveDirection;

	class IRotaryMovementPawnInterface* OwnerRotaryPawn;
};