// Fill out your copyright notice in the Description page of Project Settings.

#include "Moves.h"

#include "Components/InputComponent.h"
#include "Characters/Moves/MovesComponent.h"


FGamePawnMoves::FGamePawnMoves(UMovesComponent* InMovesComp)
{
	
	bIsEnableAxis = bIsEnableActionPressed = bIsEnableActionReleased = false;
	MovesComp = InMovesComp;
	StartSkilledness = StopSkilledness = nullptr;
	ExcuteSkilledness = nullptr;
}

void FGamePawnMoves::RebindFunction(SkillednessAction InPressedFunc, SkillednessAction InReleasedFunc, SkillednessAxis InExcuteFunc)
{
	StartSkilledness = bIsEnableActionPressed ? InPressedFunc : nullptr;
	StopSkilledness = bIsEnableActionReleased ? InReleasedFunc : nullptr;
	ExcuteSkilledness = bIsEnableAxis ? InExcuteFunc : nullptr;
}

void FGamePawnMoves::RebindInput(UInputComponent* OwnerInputComp)
{
	if (OwnerInputComp && MovesComp)
	{
		if (bIsEnableActionPressed && StartSkilledness) { OwnerInputComp->BindAction(BindingName, IE_Pressed, MovesComp, StartSkilledness); }
		if (bIsEnableActionReleased && StopSkilledness) { OwnerInputComp->BindAction(BindingName, IE_Released, MovesComp, StopSkilledness); }
		if (bIsEnableAxis && ExcuteSkilledness) { OwnerInputComp->BindAxis(BindingName, MovesComp, ExcuteSkilledness); }
	}
}