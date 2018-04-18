// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnManager.h"

class FRoleForm;
class AGamePawn;

class FRoleFormClassInfo
{
public:
	FRoleFormClassInfo(const FName& InName, RoleFormCreateFunc InCreateFunc)
	{
		CreateFunc = InCreateFunc;
		UGamePawnManager::RegisterRoleForm(InName, this);
	}

	FRoleForm* CreateObject(AGamePawn* InGamePawn)
	{
		return CreateFunc ? (*CreateFunc)(InGamePawn) : nullptr;
	}

	RoleFormCreateFunc CreateFunc;
};