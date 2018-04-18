// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingGameMode_Menu.h"

#include "EveryThingGameInstance.h"

#include "Online/EveryThingGameSession.h"
#include "Online/PlayerController_Menu.h"
#include "UI/EveryThingHUD_Menu.h"


AEveryThingGameMode_Menu::AEveryThingGameMode_Menu()
{
	GameSessionClass = AEveryThingGameSession::StaticClass();
	HUDClass = AEveryThingHUD_Menu::StaticClass();
	PlayerControllerClass = APlayerController_Menu::StaticClass();

	DefaultPawnClass = nullptr;
}