// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EveryThingTypes.h"

#include "EveryThingGameMode_Game.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API AEveryThingGameMode_Game : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AEveryThingGameMode_Game();

	//////////////////////////////////////////////////////////////////////////
	/// Game Flow Path
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC);
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginDestroy() override;

	virtual void HandlePlayerLogin(APlayerController* NewPlayer);
	virtual void HandleETGameReay();
	virtual void HandleETGameStart();
	virtual bool HandleETGameIsOver();
	virtual void HandleETGameOver();
	virtual void HandlerETBackToHouse();

	
	//////////////////////////////////////////////////////////////////////////
	/// Damage
	virtual bool CanTakeDamage(AGamePawn* PlayerPawn, AActor* OtherActor);
	virtual float GetDamageFromGamePawnHit(AGamePawn* PlayerPawn, AGamePawn* OtherPawn, const FVector& NormalInpulse, const FHitResult& Hit) const;
	virtual float GetDamageFromHitableHit(AGamePawn* PlayerPawn, const FVector& NormalInpulse, const FHitResult& Hit)const;
	virtual float GetDamageFromActorHit(AGamePawn* PlayerPawn, const FVector& NormalInpulse, const FHitResult& Hit)const;
	virtual float GetActualDamage(float InDamage, EElementType InDamageType = EElementType::None, EElementType PawnElementType = EElementType::None, float ElementResistance = 0.f) const;
	float GetDamageScaleFromElementType(EElementType CauserElementType, EElementType AcceptElementType) const;



protected:
	float NormalDamageScale;
	float SpecificToDamageScale;
	bool bBackToHome;

	TSubclassOf<class AEveryThingPlayerStart> ETPlayerStatrClass;
};
