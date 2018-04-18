// Fill out your copyright notice in the Description page of Project Settings.

#include "FootballAttackComponent.h"

#include "UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

#include "Engine.h"

#include "EveryThingAssetManager.h"
#include "Characters/GamePawns/Football/FootballPawn.h"
#include "Characters/Movement/Components/RotaryMovementComponent.h"

UFootballAttackComponent::UFootballAttackComponent()
{ 
	CommonAttack.bIsEnableActionPressed = true;
	SpecialAttack.bIsEnableActionPressed = true;
	
	RebindAll();
	
	OwnerPawn = Cast<AFootballPawn>(GetOwner());

	bIsAttacking = false;
	CurrentAttackTarget = nullptr;
	
	bIsAutoAim = true;
	MinAutoAimDistance = 500.f;
	MaxAttackDistance = 5000.f;
	HitElasticScale = 0.7f;

	UEveryThingAssetManager::GetAssetManagerInstance()->NeededParticleFromName(TEXT("Explosion"));
	
	PrimaryComponentTick.bCanEverTick = true;
}

void UFootballAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UFootballAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsAttacking)
	{
		ExcuteAttack(DeltaTime);
	}
	else
	{
		StopAttack();
	}
}


void UFootballAttackComponent::OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	Super::OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
	
	
	if (Cast<IHitAbleInterface>(OtherActor))
	{
		if (HasAuthority())
		{
			if (bIsCommonAttack && OwnerPawn && OwnerPawn->GetRotaryMovementComponent())
			{
				OwnerPawn->GetRotaryMovementComponent()->AcceptForceImpulse(Hit.Location, -HitElasticScale * OwnerPawn->GetVelocity());
			}
			
			StopAttack();
		}

		UParticleSystem*  HitEmitter = UEveryThingAssetManager::GetAssetManagerInstance()->GetParticleFromName(TEXT("Explosion"));
		if (HitEmitter) { UGameplayStatics::SpawnEmitterAtLocation(this, HitEmitter, Hit.Location); }
	}
}

void UFootballAttackComponent::StartCommonAttack() { ToggleAttack(true); }
void UFootballAttackComponent::StartSpecialAttack() { ToggleAttack(false); }


void UFootballAttackComponent::StartAttack(bool bInIsCommonAttack)
{
	if (bIsAttacking) { return; }

	AActor* AttackTarget = OwnerPawn ? OwnerPawn->TryToGetAttackTarget(MaxAttackDistance) : nullptr;

	if (AttackTarget != nullptr)
	{
		if (HasAuthority() || IsAutonomousProxy())
		{
			ServerStartAttack(bInIsCommonAttack, AttackTarget);
		}
	}
}


bool UFootballAttackComponent::ServerStartAttack_Validate(bool bInIsCommonAttack, AActor* AttackTarget) { return true; }
void UFootballAttackComponent::ServerStartAttack_Implementation(bool bInIsCommonAttack, AActor* AttackTarget) { MulticastStartAttack(bInIsCommonAttack, AttackTarget); }
bool UFootballAttackComponent::ServerStopAttack_Validate() { return true; }
void UFootballAttackComponent::ServerStopAttack_Implementation() { MulticastStopAttack(); }

void UFootballAttackComponent::MulticastStartAttack_Implementation(bool bInIsCommonAttack, AActor* AttackTarget)
{
	bIsAutoAim = true;
	bIsAttacking = true;
	bIsCommonAttack = bInIsCommonAttack;
	SetWantedToAcceptHitFunction(true);

	CurrentAttackTarget = AttackTarget;

	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UFootballAttackComponent::StopAttack()
{
	if (HasAuthority() || IsAutonomousProxy())
	{
		ServerStopAttack();
	}
}

void UFootballAttackComponent::MulticastStopAttack_Implementation()
{
	bIsAttacking = false;
	SetWantedToAcceptHitFunction(false);

	CurrentAttackTarget = nullptr;

	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UFootballAttackComponent::ExcuteAttack(float DeltaTime)
{
	URotaryMovementComponent* OwnerPawnRMC = OwnerPawn ? OwnerPawn->GetRotaryMovementComponent() : nullptr;

	if (CurrentAttackTarget && OwnerPawnRMC)
	{
		FVector AttackActorLocation;
		if (bIsAutoAim)
		{
			AttackActorLocation = CurrentAttackTarget->GetActorLocation();

			float Distance = (OwnerPawn->GetActorLocation() - AttackActorLocation).Size();
			if (Distance < MinAutoAimDistance)
			{
				bIsAutoAim = false;
				LastAttackTartgetLocation = AttackActorLocation;
				RemainingTime = Distance / OwnerPawn->GetVelocity().Size();
			}
		}
		else
		{
			AttackActorLocation = LastAttackTartgetLocation;

			RemainingTime -= DeltaTime;
			if (RemainingTime < 0.f)
			{
				DeltaTime += RemainingTime;
				StopAttack();
			}
		}
		OwnerPawnRMC->MoveToLocation(AttackActorLocation, 90.f * DeltaTime);
	}
	else
	{
		StopAttack();
	}
}


void UFootballAttackComponent::ToggleAttack(bool bInIsCommonAttack)
{
	if (bIsAttacking)
	{
		if (bInIsCommonAttack == bIsCommonAttack) { StopAttack(); }		
	}
	else
	{
		StartAttack(bInIsCommonAttack);
	}
}


void UFootballAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFootballAttackComponent, CurrentAttackTarget);
	DOREPLIFETIME(UFootballAttackComponent, LastAttackTartgetLocation);
	DOREPLIFETIME(UFootballAttackComponent, bIsCommonAttack);
	DOREPLIFETIME(UFootballAttackComponent, bIsAttacking);
	DOREPLIFETIME(UFootballAttackComponent, bIsAutoAim);
	DOREPLIFETIME(UFootballAttackComponent, HitElasticScale);
	DOREPLIFETIME(UFootballAttackComponent, MinAutoAimDistance);
	DOREPLIFETIME(UFootballAttackComponent, MaxAttackDistance);
}