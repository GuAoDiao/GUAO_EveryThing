// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Form/GamePawnFormClassInfo.h"

class AGamePawn;
class USkillComponent;
class UAttackComponent;

#define DECLARE_GAMEPAWNFORM_CLASS(FormName) \
protected: \
	const static FRoleFormClassInfo FormClassInfo; \
public: \
	virtual const FRoleFormClassInfo* GetGamePawnSkinClassInfo() const; \
	static FRoleForm* CreateGamePawnForm(class AGamePawn* InGamePawn);

#define IMPLEMENT_GAMEPAWNFORM_CLASS(FormName, FormClass) \
	const FRoleFormClassInfo FormClass::FormClassInfo = FRoleFormClassInfo(FormName, &FormClass::CreateGamePawnForm); \
	const FRoleFormClassInfo* FormClass::GetGamePawnSkinClassInfo() const {return &FormClass::FormClassInfo; } \
	FRoleForm* FormClass::CreateGamePawnForm(class AGamePawn* InGamePawn) {return new FormClass(InGamePawn);}


class GUAO_EVERYTHING_API FRoleForm
{	
public:
	FRoleForm(AGamePawn* InGamePawn);

	virtual void LoadGamePawnForm();
	void UnloadGamePawnForm();
protected:
	AGamePawn* OwnerGamePawn;

	UClass* SkillCompClass;
	UClass* AttackCompClass;
};
