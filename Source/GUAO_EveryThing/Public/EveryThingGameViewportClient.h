// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "EveryThingGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UEveryThingGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual void PostRender(UCanvas* Canvas) override;

	void ClearFade();
	void Fade(const float Duration, const bool bInToBlack);
	void DrawScreenFade(UCanvas* Canvas);

private:
	bool bFading;
	bool bToBlack;
	float FadAlpha;
	float FadeStartTime;
	float FadeDuration;	
};
