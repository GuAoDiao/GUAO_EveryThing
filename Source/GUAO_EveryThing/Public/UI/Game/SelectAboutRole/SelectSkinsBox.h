// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveryThingTypes.h"
#include "SelectSkinsBox.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API USelectSkinsBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeSelectSkinsBoxDisplay(const FName& TargetRoleName);

	UFUNCTION(BlueprintImplementableEvent)
	void AddSelectItem(class USelectItem* SelectItem);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSkinItemIsHave(const TArray<FName>& AllHaveSkinFormName);

	void OnRoleNameUpdate(const FName& RoleName);	

	void OnAllHaveRoleSkinNameUpdate(const TArray<FName>& AllHaveSkinFormName);
};
