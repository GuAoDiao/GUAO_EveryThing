// Fill out your copyright notice in the Description page of Project Settings.

#include "RotaryMovementComponent.h"

#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Online/PlayerController_Game.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Characters/GamePawn.h"
#include "Characters/Movement/Interfaces/RotaryMovementPawnInterface.h"

#define LOCTEXT_NAMESPACE "Everything_Characters_RotaryMovementComponent"

URotaryMovementComponent::URotaryMovementComponent()
{	
	bIsJumping = false;
	bIsFastMovementState = false;

	FastSpeedScale = 1.5f;
	FastJumpScale = 1.5f;
	
	OwnerRotaryPawn = Cast<IRotaryMovementPawnInterface>(GetOwner());
	OwnerPrimitiveComp = OwnerRotaryPawn ? OwnerRotaryPawn->GetPrimitiveComponent() : nullptr;

	bWantsToMove = false;
	WantMoveDirection = FVector::ZeroVector;
}

void URotaryMovementComponent::BeginDestroy()
{
	CloseMoveTimer();

	Super::BeginDestroy();
}

void URotaryMovementComponent::StartMoveTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.TimerExists(MoveTimer))
	{
		TimerManager.SetTimer(MoveTimer, this, &URotaryMovementComponent::MoveTimerImplementation, 0.16f, true);
	}
	else if(TimerManager.IsTimerPaused(MoveTimer))
	{
		TimerManager.UnPauseTimer(MoveTimer);
	}
}

void URotaryMovementComponent::PauseMoveTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.PauseTimer(MoveTimer);

}
void URotaryMovementComponent::UnpauseMoveTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.UnPauseTimer(MoveTimer);
}

void URotaryMovementComponent::CloseMoveTimer()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
	}
}

void URotaryMovementComponent::MoveTimerImplementation()
{
	if (bWantsToMove && OwnerGamePawn)
	{
		FVector ActualDirection = FVector::ZeroVector;
		if (WantMoveDirection.X != 0.f) { ActualDirection += WantMoveDirection.X * OwnerRotaryPawn->GetActualForwardVector(); }
		if (WantMoveDirection.Y != 0.f) { ActualDirection += WantMoveDirection.Y * OwnerRotaryPawn->GetActualRightVector(); }
		ActualDirection.Z = 0.f;

		if (ActualDirection != FVector::ZeroVector)
		{
			Move(ActualDirection.GetSafeNormal(), 2.f);
		}
		else
		{
			bWantsToMove = false;
			PauseMoveTimer();
		}
		
		WantMoveDirection = FVector::ZeroVector;
	}
	else
	{
		bWantsToMove = false;
		PauseMoveTimer();
	}
}

void URotaryMovementComponent::RebindInputComp(class UInputComponent* OwnerInputComp)
{
	Super::RebindInputComp(OwnerInputComp);

	if (!OwnerInputComp) { return; }

	OwnerInputComp->BindAxis("MoveForward", this, &URotaryMovementComponent::MoveForward);
	OwnerInputComp->BindAxis("MoveRight", this, &URotaryMovementComponent::MoveRight);

	OwnerInputComp->BindAction("Jump", IE_Pressed, this, &URotaryMovementComponent::StartJump);
	OwnerInputComp->BindAction("ToggleMovementState", IE_Pressed, this, &URotaryMovementComponent::ToogleMovementState);
}

void URotaryMovementComponent::UpdateAgilityAndQuality(float Agility, float Quality, float QualityScale)
{
	Super::UpdateAgilityAndQuality(Agility, Quality, QualityScale);

	SpeedForceBase = ActualMoveForce * ActualSpeed * 1.5f;
	JumpForceBase = ActualJumpForce * 1.5f;

	SetMovementState(bIsFastMovementState);
}


void URotaryMovementComponent::MoveForward(float AxisValue)
{
	if (OwnerRotaryPawn && AxisValue!= 0.f)
	{
		WantMoveDirection.X = AxisValue;

		if (!bWantsToMove && GetWorld())
		{
			bWantsToMove = true;

			StartMoveTimer();
		}
	}
}

void URotaryMovementComponent::MoveRight(float AxisValue)
{
	if (OwnerRotaryPawn && AxisValue != 0.f)
	{
		WantMoveDirection.Y = AxisValue;

		if (!bWantsToMove && GetWorld())
		{
			bWantsToMove = true;

			StartMoveTimer();
		}
	}
}

void URotaryMovementComponent::Move(const FVector& Direction, float AxisValue)
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerMove(Direction, AxisValue);
	}

	if (OwnerPrimitiveComp)
	{
		AddForceIfHaveEnoughStamina(Direction * CurrentSpeed * AxisValue);
	}
}

bool URotaryMovementComponent::ServerMove_Validate(const FVector& Direction, float AxisValue) { return true; }
void URotaryMovementComponent::ServerMove_Implementation(const FVector& Direction, float AxisValue) { Move(Direction, AxisValue); }

void URotaryMovementComponent::MoveToLocation(const FVector& Location, float AxisValue)
{
	if (OwnerPrimitiveComp) { Move(UKismetMathLibrary::FindLookAtRotation(OwnerPrimitiveComp->GetComponentLocation(), Location).Vector(), AxisValue); }
}

void URotaryMovementComponent::AcceptForceImpulse(const FVector& Location, const FVector& Force)
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerAcceptForceImpulse(Location, Force);
	}

	if (OwnerPrimitiveComp) { OwnerPrimitiveComp->AddImpulseAtLocation(Force, Location); }
}
bool URotaryMovementComponent::ServerAcceptForceImpulse_Validate(const FVector& Location, const FVector& Force) { return true; }
void URotaryMovementComponent::ServerAcceptForceImpulse_Implementation(const FVector& Location, const FVector& Force) { AcceptForceImpulse(Location, Force); }

void URotaryMovementComponent::StartJump()
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerStartJump();
	}

	if (CanJump() && OwnerRotaryPawn && OwnerPrimitiveComp)
	{
		if (AddImpulseIfHaveEnoughStamina(OwnerRotaryPawn->GetActualUpVector() * CurrentJumpForce))
		{
			SetIsJumping(true);
		}
	}
}
bool URotaryMovementComponent::ServerStartJump_Validate() { return true; }
void URotaryMovementComponent::ServerStartJump_Implementation() { StartJump(); }

void URotaryMovementComponent::ToogleMovementState()
{
	if (!HasAuthority() && IsAutonomousProxy())
	{
		ServerToogleMovementState();
	}
	else
	{
		bIsFastMovementState = !bIsFastMovementState;
		SetMovementState(bIsFastMovementState);

		APlayerController_Game* OwnerPC_G = Cast<APlayerController_Game>(OwnerGamePawn->GetController());
		if (OwnerPC_G)
		{
			OwnerPC_G->ClientCreatePlayerFightInfo(
				bIsFastMovementState ? LOCTEXT("OnMovementFastState", "Toggle To Fast State") : LOCTEXT("OnMovementSlowState", "Toggle To Slow State")
			);
		}
	}
}

void URotaryMovementComponent::SetMovementState(bool bInIsFastMovementState)
{	
	if (bInIsFastMovementState)
	{
		CurrentSpeed = SpeedForceBase * FastSpeedScale;
		CurrentJumpForce = JumpForceBase * FastJumpScale;
	}
	else
	{
		CurrentSpeed = SpeedForceBase;
		CurrentJumpForce = JumpForceBase;
	}
}

bool URotaryMovementComponent::ServerToogleMovementState_Validate() { return true; }
void URotaryMovementComponent::ServerToogleMovementState_Implementation() { ToogleMovementState(); }

void URotaryMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URotaryMovementComponent, CurrentSpeed);
	DOREPLIFETIME(URotaryMovementComponent, CurrentJumpForce);	

	DOREPLIFETIME(URotaryMovementComponent, bIsJumping);
	DOREPLIFETIME(URotaryMovementComponent, bIsFastMovementState);
}

#undef LOCTEXT_NAMESPACE