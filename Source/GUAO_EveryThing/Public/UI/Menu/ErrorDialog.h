// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorDialog.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UErrorDialog : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetErrorMessage(const FText& InMessage);

	UFUNCTION(BlueprintImplementableEvent)
	FText GetErrorMessage() const;

	UFUNCTION(BlueprintCallable)
	void Close();
};
