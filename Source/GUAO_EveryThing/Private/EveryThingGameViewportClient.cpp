// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameViewportClient.h"


#include "Engine/World.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

void UEveryThingGameViewportClient::PostRender(UCanvas* Canvas)
{
	Super::PostRender(Canvas);

	if (bFading) { DrawScreenFade(Canvas); }
}

void UEveryThingGameViewportClient::ClearFade()
{
	bFading = false;
}
void UEveryThingGameViewportClient::Fade(const float Duration, const bool bInToBlack)
{
	if (World)
	{
		bFading = true;
		bToBlack = bInToBlack;
		FadeDuration = Duration;
		FadeStartTime = World->GetTimeSeconds();
	}
}
void UEveryThingGameViewportClient::DrawScreenFade(UCanvas* Canvas)
{
	if (bFading && World && Canvas)
	{
		float Time = World->GetTimeSeconds();
		float Alpha = FMath::Clamp((Time - FadeStartTime) / FadeDuration, 0.f, 1.f);
		if (Alpha == 1.f && !bToBlack)
		{
			bFading = false;
		}
		else
		{
			FColor OldColor = Canvas->DrawColor;

			FLinearColor FadeColor = FLinearColor::Black;
			FadeColor.A = bToBlack ? Alpha : 1 - Alpha;
			Canvas->DrawColor = FadeColor.ToFColor(true);
			// draw mask layer to implement fade in/out
			Canvas->DrawTile(Canvas->DefaultTexture, 0.f, 0.f, Canvas->ClipX, Canvas->ClipY, 0.f, 0.f, Canvas->DefaultTexture->GetSizeX(), Canvas->DefaultTexture->GetSizeY());

			Canvas->DrawColor = OldColor;
		}
	}
}