// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Moves/Moves.h"

#include "MovesComponent.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UMovesComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UMovesComponent();

public:
	virtual void RebindInputComp(class UInputComponent* OwnerInputComp) {}

protected:
	bool HasAuthority();
	bool IsAutonomousProxy();

	virtual void RebindAll() {};
public:
	void SetWantedToAcceptHitFunction(bool bWanted);
	FDelegateHandle OnHitDelegateHandle;
	// execute when bWantedToAcceptHitFunction is true
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit) { OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit); }
	virtual void OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit) {};
};
