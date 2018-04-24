// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "ChatWindowPlayerStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChatWindowPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GUAO_EVERYTHING_API IChatWindowPlayerStateInterface
{
	GENERATED_BODY()

public:
	virtual const FString& GetPlayerChatName() const = 0;
	virtual int32 GetPlayerChatID() const = 0;
	virtual int32 GetPlayerChatTeamID() const = 0;
};
