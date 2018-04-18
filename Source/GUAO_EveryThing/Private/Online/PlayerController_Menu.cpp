// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController_Menu.h"

void APlayerController_Menu::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		// for Role 3D Display
		InputComponent->BindAxis("MoveForward");
		InputComponent->BindAxis("MoveRight");
	}
}