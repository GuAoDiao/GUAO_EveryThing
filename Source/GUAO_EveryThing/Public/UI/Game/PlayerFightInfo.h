// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerFightInfo.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UPlayerFightInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ClientCreatePlayerFightInfo(const FText& FightInfo);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InitializePlayerFightInfo();
	
	void OnAcceptCure(const FString& CauserName, float Treatment);
	void OnAcceptDamage(const FString& CauserName, float Damage);
	void OnAcceptCriticalDamage(const FString& CauserName);
	void OnTakeCure(const FString& AccepterName, float Treatment);
	void OnTakeDamage(const FString& AccepterName, float Damage);
	void OnTakeCriticalDamage(const FString& AccepterName);
	void OnBeKilled(const FString& KillerName);
	void OnSuicided();
	void OnKillOther(const FString& KilledName);

	//////////////////////////////////////////////////////////////////////////
	/// Player Fight Info Item List
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ScrollVerticalBoxToEnd();

	void AddPlayerFightInfoItem(const FText& FightInfo, float InRamainingTime = 5.f);
	
	UPROPERTY(BlueprintReadWrite)
	class UVerticalBox* PlayerFightInfoVerticalBox;

	//////////////////////////////////////////////////////////////////////////
	/// Tidy
protected:
	void TidyPlayerFightInfo();

	float TidyRate;
	FTimerHandle TidyTimer;	
};
