// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveryThingTypes.h"
#include "SelectFormsBox.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API USelectFormsBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeSelectFormsBoxDisplay(const FName& TargetRoleName);

	UFUNCTION(BlueprintImplementableEvent)
	void AddSelectItem(class USelectItem* SelectItem);


	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFormItemIsHave(const TArray<FName>& AllHaveRoleFormName);

	void OnRoleNameUpdate(const FName& RoleName);

	void OnAllHaveRoleFormNameUpdate(const TArray<FName>& AllHaveRoleFormName);
};
