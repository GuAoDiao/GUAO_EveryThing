// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_Menu.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API APlayerController_Menu : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
	
	
};
