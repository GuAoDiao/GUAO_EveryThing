// Fill out your copyright notice in the Description page of Project Settings.

#include "ETGM_TeamModel.h"

#include "Online/TeamModel/EveryThingPlayerStart_Team.h"


AETGM_TeamModel::AETGM_TeamModel()
{
	ETPlayerStatrClass = AEveryThingPlayerStart_Team::StaticClass();
}


bool AETGM_TeamModel::CanTakeDamage(AGamePawn* PlayerPawn, AActor* OtherActor)
{
	return true;
}