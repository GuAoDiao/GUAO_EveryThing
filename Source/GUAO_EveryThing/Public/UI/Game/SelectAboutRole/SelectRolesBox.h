// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "SelectRolesBox.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API USelectRolesBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSelectRolesBoxDisplay(const TArray<FName>& AllHaveRoleNames);

	UFUNCTION(BlueprintImplementableEvent)
	void AddSelectItem(class USelectItem* SelectItem);

	void OnPlayerInfoUpdate(const FPlayerInfo& InPlayerInfo);
	void OnPlayerStateUpdate(APlayerState* PlayerState);
};
