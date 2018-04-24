// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Skin/GamePawnSkinClassInfo.h"

class UStaticMeshComponent;


#define DECLARE_GAMEPAWNSKIN_CLASS(SkinName) \
protected: \
	const static FRoleSkinClassInfo SkinClassInfo; \
public: \
	virtual const FRoleSkinClassInfo* GetGamePawnSkinClassInfo() const; \
	static FRoleSkin* CreateGamePawnSkin(class UStaticMeshComponent* InStaticMeshComp);

#define IMPLEMENT_GAMEPAWNSKIN_CLASS(SkinName, SkinClass) \
	const FRoleSkinClassInfo SkinClass::SkinClassInfo = FRoleSkinClassInfo(SkinName, &SkinClass::CreateGamePawnSkin); \
	const FRoleSkinClassInfo* SkinClass::GetGamePawnSkinClassInfo() const {return &SkinClass::SkinClassInfo; } \
	FRoleSkin* SkinClass::CreateGamePawnSkin(class UStaticMeshComponent* InStaticMeshComp) {return new SkinClass(InStaticMeshComp);}

class GUAO_EVERYTHING_API FRoleSkin
{	
public:
	FRoleSkin(UStaticMeshComponent* StaticMeshComp);
	virtual ~FRoleSkin() {}
	
	void LoadAllGameSkinMaterial();

	virtual void LoadGamePawnSkin();

	void UnloadGamePawnSkin();

protected:
	UStaticMeshComponent* OwnerStaticMeshComp;

	TArray<FName> MaterialNames;
};
