// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerChairPawn.h"

#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"

#include "Characters/PlayerPawnComponent.h"
#include "Characters/Movement/Components/JumpMovementComponent.h"

APlayerChairPawn::APlayerChairPawn()
{
	OwnerPlayerPawnComponent = CreateDefaultSubobject<UPlayerPawnComponent>(TEXT("PlayerPawnComponent"));
}

UPlayerPawnComponent* APlayerChairPawn::GetPlayerPawnComponent() const { return OwnerPlayerPawnComponent; }

void APlayerChairPawn::Tick(float DeltaTime)
{
	APlayerController* OwnerPC = Cast<APlayerController>(GetController());

	if (OwnerPC && OwnerPC->IsLocalController())
	{
		int32 Width, Height;
		OwnerPC->GetViewportSize(Width, Height);

		FVector OwnerDirection, CenterDirection;
		OwnerPC->DeprojectScreenPositionToWorld(Width * 0.5f, Height * 0.5f, OwnerDirection, CenterDirection);
		OwnerDirection = StaticMeshComp->GetForwardVector();

		CenterDirection.Z = 0.f; OwnerDirection.Z = 0.f;

		float Value = FVector::CrossProduct(OwnerDirection.GetSafeNormal(), CenterDirection.GetSafeNormal()).Z;
		if (Value > 0.1f)
		{
			GetJumpMovementComponent()->RotatePawn(FMath::Lerp(Value, 1.f, 0.5f) * DeltaTime * 150.f);
		}
		else if (Value < -0.1f)
		{
			GetJumpMovementComponent()->RotatePawn(FMath::Lerp(Value, -1.f, 0.5f) * DeltaTime * 150.f);
		}
	}
}