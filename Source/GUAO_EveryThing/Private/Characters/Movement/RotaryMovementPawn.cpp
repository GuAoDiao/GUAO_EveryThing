// Fill out your copyright notice in the Description page of Project Settings.

#include "RotaryMovementPawn.h"

#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

#include "Characters/Movement/Components/RotaryMovementComponent.h"

ARotaryMovementPawn::ARotaryMovementPawn()
{
	MovementComp = CreateDefaultSubobject<URotaryMovementComponent>(TEXT("RotaryMovementComponent"));
}



//////////////////////////////////////////////////////////////////////////
/// IRotaryMovementPawnInterface

/// For Controller
UPrimitiveComponent* ARotaryMovementPawn::GetPrimitiveComponent() const { return StaticMeshComp; }


/// For Rotary Movement

URotaryMovementComponent* ARotaryMovementPawn::GetRotaryMovementComponent() const
{
	URotaryMovementComponent* OwnerRotaryMovementComponent = Cast<URotaryMovementComponent>(MovementComp);
	check(OwnerRotaryMovementComponent);
	return OwnerRotaryMovementComponent;
}

const FVector ARotaryMovementPawn::GetActualForwardVector() const { return GetControlRotation().Vector(); }
const FVector ARotaryMovementPawn::GetActualRightVector() const { return FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);}
const FVector ARotaryMovementPawn::GetActualUpVector() const { return FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Z); }

//////////////////////////////////////////////////////////////////////////
/// Hit
void ARotaryMovementPawn::OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	Super::OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);

	GetRotaryMovementComponent()->SetIsJumping(false);
}