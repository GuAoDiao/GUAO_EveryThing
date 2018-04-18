// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "ChatWindowGameStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChatWindowGameStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GUAO_EVERYTHING_API IChatWindowGameStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual const FString& GetPlayerChatName(int32 PlayerID) const = 0;
	virtual const TArray<class APlayerState*>& GetAllChatPlayerState() const = 0;
};
