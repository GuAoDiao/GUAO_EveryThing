// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GamePawnManager.h"

class FRoleSkin;

class FRoleSkinClassInfo
{
public:
	FRoleSkinClassInfo(const FName& InName, RoleSkinCreateFunc InCreateFunc)
	{
		CreateFunc = InCreateFunc;
		UGamePawnManager::RegisterRoleSkin(InName, this);
	}

	FRoleSkin* CreateObject(UStaticMeshComponent* InStaticMeshComp)
	{
		return CreateFunc ? (*CreateFunc)(InStaticMeshComp) : nullptr;
	}

	RoleSkinCreateFunc CreateFunc;
};