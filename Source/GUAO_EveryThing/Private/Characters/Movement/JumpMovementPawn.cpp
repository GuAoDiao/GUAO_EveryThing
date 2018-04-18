// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpMovementPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


#include "Characters/Movement/Components/JumpMovementComponent.h"

AJumpMovementPawn::AJumpMovementPawn()
{
	MovementComp = CreateDefaultSubobject<UJumpMovementComponent>(TEXT("JumpMovementComp"));

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollisionComp->SetRelativeLocation(FVector(0.f, 0.f, -83.f));
	BoxCollisionComp->SetBoxExtent(FVector(45.f, 45.f, 1.f));

	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollisionComp->SetCollisionResponseToAllChannels(ECR_Block);

	BoxCollisionComp->SetNotifyRigidBodyCollision(true);
	BoxCollisionComp->OnComponentHit.AddDynamic(this, &AJumpMovementPawn::OnBoxCompHit);

	BoxCollisionComp->SetupAttachment(GetRootComponent());
}

//////////////////////////////////////////////////////////////////////////
/// IRotaryMovementPawnInterface

/// For Controller
UJumpMovementComponent* AJumpMovementPawn::GetJumpMovementComponent() const
{
	UJumpMovementComponent* OwnerJumpMovementComp = Cast<UJumpMovementComponent>(MovementComp);
	check(OwnerJumpMovementComp);
	return OwnerJumpMovementComp;
}
/// For Rotary Movement
UPrimitiveComponent* AJumpMovementPawn::GetPrimitiveComponent() const
{
	return StaticMeshComp;
};


//////////////////////////////////////////////////////////////////////////
/// Hit
void AJumpMovementPawn::OnBoxCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	GetJumpMovementComponent()->SetCanJump(true);
}