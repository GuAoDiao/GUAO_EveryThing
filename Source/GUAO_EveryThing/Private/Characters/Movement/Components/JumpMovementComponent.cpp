// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpMovementComponent.h"

#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "UnrealNetwork.h"

#include "Characters/GamePawn.h"
#include "Characters/Movement/Interfaces/JumpMovementPawnInterface.h"

UJumpMovementComponent::UJumpMovementComponent()
{
	bReplicates = true;
	
	AdjustRotationForce = 60.f;
	AtuoAdjustRotationForceStrength = 1.f;

	AdjustPawnRotationForce = 20.f;

	bHasMoveDirection = false;
	bIsToogleMovementState = false;
	bCanJump = true;

	OwnerJumpPawn = Cast<IJumpMovementPawnInterface>(GetOwner());
	OwnerPrimitiveComp = OwnerJumpPawn ? OwnerJumpPawn->GetPrimitiveComponent() : nullptr;

	PrimaryComponentTick.bCanEverTick = true;
}

void UJumpMovementComponent::RebindInputComp(UInputComponent* OwnerInputComp)
{
	Super::RebindInputComp(OwnerInputComp);

	if (!OwnerInputComp) { return; }

	OwnerInputComp->BindAxis("MoveForward", this, &UJumpMovementComponent::AdjustForwardPosition);
	OwnerInputComp->BindAxis("MoveRight", this, &UJumpMovementComponent::AdjustRightPosition);


	OwnerInputComp->BindAction("MoveForwardAction", IE_Pressed, this, &UJumpMovementComponent::JumpMoveToForward);
	OwnerInputComp->BindAction("MoveBackAction", IE_Pressed, this, &UJumpMovementComponent::JumpMoveToBack);
	OwnerInputComp->BindAction("MoveLeftAction", IE_Pressed, this, &UJumpMovementComponent::JumpMoveToLeft);
	OwnerInputComp->BindAction("MoveRightAction", IE_Pressed, this, &UJumpMovementComponent::JumpMoveToRight);

	OwnerInputComp->BindAction("MoveForwardAction", IE_Released, this, &UJumpMovementComponent::ClearMoveDirection);
	OwnerInputComp->BindAction("MoveBackAction", IE_Released, this, &UJumpMovementComponent::ClearMoveDirection);
	OwnerInputComp->BindAction("MoveLeftAction", IE_Released, this, &UJumpMovementComponent::ClearMoveDirection);
	OwnerInputComp->BindAction("MoveRightAction", IE_Released, this, &UJumpMovementComponent::ClearMoveDirection);

	OwnerInputComp->BindAction("Jump", IE_Pressed, this, &UJumpMovementComponent::StartJump);
	OwnerInputComp->BindAction("Jump", IE_Released, this, &UJumpMovementComponent::StopJump);

	OwnerInputComp->BindAction("ToggleMovementState", IE_Pressed, this, &UJumpMovementComponent::BeginToggleMovementState);
	OwnerInputComp->BindAction("ToggleMovementState", IE_Released, this, &UJumpMovementComponent::EndToggleMovementState);
}

void UJumpMovementComponent::UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale)
{
	Super::UpdateAgilityAndQuality(Agility, Quality, QualityScale);

	JumpForwardForce = ActualMoveForce * ActualSpeed * 25.f;
	JumpHeightForce = ActualJumpForce * 2.5f;

	AdjustLocationForce = ActualMoveForce * ActualSpeed * 0.35f;
}

void UJumpMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (OwnerPrimitiveComp) { AutoAdjsutRotationPosition(DeltaTime); }
}

//////////////////////////////////////////////////////////////////////////
/// Adjust Position

void UJumpMovementComponent::AdjustForwardPosition(float AxisValue) { AdjustPosition(!bIsToogleMovementState, true, AxisValue); }
void UJumpMovementComponent::AdjustRightPosition(float AxisValue) { AdjustPosition(!bIsToogleMovementState, false, AxisValue); }

void UJumpMovementComponent::AutoAdjsutRotationPosition(float DeltaTime)
{
	if (OwnerPrimitiveComp)
	{
		FRotator OwnerRotation = OwnerPrimitiveComp->GetComponentRotation();
	
		FVector Force = FVector::ZeroVector;

		if (OwnerRotation.Roll > 10.f)
		{
			Force += OwnerPrimitiveComp->GetForwardVector() * FMath::Lerp(10.f, OwnerRotation.Roll, 0.3f);
		}
		else if (OwnerRotation.Roll < -10.f)
		{
			Force += OwnerPrimitiveComp->GetForwardVector() * FMath::Lerp(-10.f, OwnerRotation.Roll, 0.3f);
		}

		if (OwnerRotation.Pitch > 10.f)
		{
			Force += OwnerPrimitiveComp->GetRightVector() * FMath::Lerp(10.f, OwnerRotation.Pitch, 0.3f);
		}
		else if (OwnerRotation.Pitch < -10.f)
		{
			Force += OwnerPrimitiveComp->GetRightVector() * FMath::Lerp(-10.f, OwnerRotation.Pitch, 0.3f);
		}

		if (Force != FVector::ZeroVector)
		{
			AddTorqueInRadiansIfHaveEnoughStamina(Force * AdjustRotationForce  * DeltaTime  * AtuoAdjustRotationForceStrength);
		}
	}
}


void UJumpMovementComponent::AdjustPosition(bool bIsAdjsutLocation, bool bIsForward, float AxisValue)
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerAdjustPosition(bIsAdjsutLocation, bIsForward, AxisValue);
	}
		
	if (OwnerPrimitiveComp)
	{
		if (bIsAdjsutLocation)
		{
			FVector Direction = bIsForward ? OwnerPrimitiveComp->GetForwardVector() : OwnerPrimitiveComp->GetRightVector();
			AddForceIfHaveEnoughStamina(Direction * AdjustLocationForce * AxisValue);
		}
		else
		{
			FVector Direction = bIsForward ? OwnerPrimitiveComp->GetRightVector() : -OwnerPrimitiveComp->GetForwardVector();
			AddTorqueInRadiansIfHaveEnoughStamina(Direction * AdjustRotationForce * AxisValue);
		}
	}
}

bool UJumpMovementComponent::ServerAdjustPosition_Validate(bool bIsAdjsutLocation, bool bIsForward, float AxisValue) { return true; }
void UJumpMovementComponent::ServerAdjustPosition_Implementation(bool bIsAdjsutLocation, bool bIsForward, float AxisValue) { AdjustPosition(bIsAdjsutLocation, bIsForward, AxisValue); }

void UJumpMovementComponent::RotatePawn(float AxisValue)
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerRotatePawn(AxisValue);
	}
	
	if (OwnerPrimitiveComp)
	{
		FVector Impulse = OwnerPrimitiveComp->GetUpVector() * 15000.f;
		FVector Torque = OwnerPrimitiveComp->GetUpVector() * AdjustPawnRotationForce * AxisValue;

		if (AddTorqueInRadiansIfHaveEnoughStamina(Torque))
		{
			OwnerPrimitiveComp->AddImpulse(Impulse);
		}
	}
}
bool UJumpMovementComponent::ServerRotatePawn_Validate(float AxisValue) { return true; }
void UJumpMovementComponent::ServerRotatePawn_Implementation(float AxisValue) { RotatePawn(AxisValue); }


//////////////////////////////////////////////////////////////////////////
/// Jump And Jump Move

inline void UJumpMovementComponent::JumpMoveToForward() { SetJumpMoveDirection(true, true); }
inline void UJumpMovementComponent::JumpMoveToBack() { SetJumpMoveDirection(true, false); }
inline void UJumpMovementComponent::JumpMoveToRight() { SetJumpMoveDirection(false, true); }
inline void UJumpMovementComponent::JumpMoveToLeft() { SetJumpMoveDirection(false, false); }

void UJumpMovementComponent::SetJumpMoveDirection(bool bIsForward, bool bIsPositive)
{
	bHasMoveDirection = true;
	if (OwnerPrimitiveComp)
	{
		WantedMoveDirection = bIsForward ? OwnerPrimitiveComp->GetForwardVector() : OwnerPrimitiveComp->GetRightVector();

		if (!bIsPositive) { WantedMoveDirection = -WantedMoveDirection; }
	}
}

void UJumpMovementComponent::ClearMoveDirection()
{
	bHasMoveDirection = false;
}

void UJumpMovementComponent::StartJump()
{
	if (bHasMoveDirection)
	{
		JumpMove(WantedMoveDirection);
	}
	else
	{
		Jump();
	}
}
void UJumpMovementComponent::StopJump()
{
}


void UJumpMovementComponent::Jump()
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerJump();
	}

	if (OwnerPrimitiveComp && bCanJump)
	{		
		if (AddImpulseIfHaveEnoughStamina(OwnerPrimitiveComp->GetUpVector() * JumpHeightForce))
		{
			bCanJump = false;
		}
	}
}
bool UJumpMovementComponent::ServerJump_Validate() { return true; }
void UJumpMovementComponent::ServerJump_Implementation() { Jump(); }


void UJumpMovementComponent::JumpMove(const FVector& Dircetion)
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerJumpMove(Dircetion);
	}

	if (OwnerPrimitiveComp && bCanJump)
	{
		if (AddImpulseIfHaveEnoughStamina(OwnerPrimitiveComp->GetUpVector() * JumpHeightForce + WantedMoveDirection * JumpForwardForce))
		{
			bCanJump = false;
		}
	}
}
bool UJumpMovementComponent::ServerJumpMove_Validate(const FVector& Dircetion) { return true; }
void UJumpMovementComponent::ServerJumpMove_Implementation(const FVector& Dircetion) { JumpMove(Dircetion); }


void UJumpMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UJumpMovementComponent, bCanJump);

	DOREPLIFETIME(UJumpMovementComponent, JumpForwardForce);
	DOREPLIFETIME(UJumpMovementComponent, JumpHeightForce);

	DOREPLIFETIME(UJumpMovementComponent, AdjustLocationForce);
	DOREPLIFETIME(UJumpMovementComponent, AdjustRotationForce);
	DOREPLIFETIME(UJumpMovementComponent, AdjustPawnRotationForce);

	DOREPLIFETIME(UJumpMovementComponent, AtuoAdjustRotationForceStrength);
}