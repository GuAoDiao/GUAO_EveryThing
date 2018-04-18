// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePropItem.generated.h"

UCLASS()
class GUAO_EVERYTHING_API ABasePropItem : public AActor
{
	GENERATED_BODY()
	
public:
	ABasePropItem();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USphereComponent* SphereComp;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float RespawnTime;

	FTimerHandle RespawnTimer;
	void Respawn();

	UFUNCTION()
	void OnPropBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual bool BeOverlapByGamePawn(class AGamePawn* OverlapGamePawn) { return true; }
	
	void CreatePlayerFightInfoFromProp(AGamePawn* TargetGamePawn, const FText& FightInfo);
};
