// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "EveryThingGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(config = EveryThing)
class GUAO_EVERYTHING_API UEveryThingGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UEveryThingGameUserSettings();

	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	
	virtual void SetToDefaults() override;
	

public:
	int32 GetGraphicsQuality() const { return GraphicsQuality; }
	void SetGraphicsQuality(int32 InGraphicsQuality) { GraphicsQuality = InGraphicsQuality; }

	bool IsLanMatch() const { return bIsLANMatch; }
	void SetLanMatch(bool bInIsLanMatch) { bIsLANMatch = bInIsLanMatch; }

	bool IsDedicatedServer() const { return bIsDedicatedServer; }
	void SetDedicatedServer(bool bInIsDedicatedServer) { bIsDedicatedServer = bInIsDedicatedServer; }
private:
	// 1 = Low, 2 = middle, 3 = Hight
	UPROPERTY(config)
	int32 GraphicsQuality;

	UPROPERTY(config)
	bool bIsLANMatch;

	UPROPERTY(config)
	bool bIsDedicatedServer;
};
