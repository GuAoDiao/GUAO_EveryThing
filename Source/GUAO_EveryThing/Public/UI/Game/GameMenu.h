// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable)
	void Confing();

	UFUNCTION(BlueprintCallable)
	void ExitGame();

	UFUNCTION(BlueprintCallable)
	void Backup();
private:
	TArray<FKey> EscapeKeys;
};
