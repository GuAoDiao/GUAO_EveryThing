// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePropItem.h"

#include "TimerManager.h"
#include "Components/SphereComponent.h"

#include "Characters/GamePawn.h"
#include "Online/PlayerController_Game.h"

ABasePropItem::ABasePropItem()
{
	bReplicates = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetSphereRadius(25.f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	SphereComp->bGenerateOverlapEvents = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABasePropItem::OnPropBeginOverlap);
	SetRootComponent(SphereComp);

	RespawnTime = 30.f;
}

void ABasePropItem::OnPropBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AGamePawn* OwnerGamePawn = Cast<AGamePawn>(OtherActor);
		if (OwnerGamePawn)
		{
			if (BeOverlapByGamePawn(OwnerGamePawn))
			{
				SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				SetActorHiddenInGame(true);

				GetWorldTimerManager().SetTimer(RespawnTimer, this, &ABasePropItem::Respawn, RespawnTime, false);
			}
		}
	}
}

void ABasePropItem::Respawn()
{
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SetActorHiddenInGame(false);
}


void ABasePropItem::CreatePlayerFightInfoFromProp(AGamePawn* TargetGamePawn, const FText& FightInfo)
{
	APlayerController_Game* TargetPC_G = TargetGamePawn ? Cast<APlayerController_Game>(TargetGamePawn->GetController()) : nullptr;
	if (TargetPC_G)
	{
		TargetPC_G->ClientCreatePlayerFightInfo(FightInfo);
	}
}

