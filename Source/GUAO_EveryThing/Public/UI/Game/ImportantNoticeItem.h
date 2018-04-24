// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EveryThingTypes.h"

#include "ImportantNoticeItem.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UImportantNoticeItem : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeImportantNoticeItem(const FText& ImportantNoteText, EImportantNoteType NoteType);
	
	float RemainingTime;
};
