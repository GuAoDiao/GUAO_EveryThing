// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveryThingTypes.h"
#include "GamePawnManager.generated.h"

class AGamePawn;
class FRoleSkin;
class FRoleForm;
class UStaticMeshComponent;
class FRoleSkinClassInfo;
class FRoleFormClassInfo;

typedef FRoleSkin*(*RoleSkinCreateFunc)(UStaticMeshComponent* /*InStatichMeshComp*/);
typedef FRoleForm*(*RoleFormCreateFunc)(AGamePawn* /*InGamePawn*/);

/**
 * 
 */
UCLASS()
class GUAO_EVERYTHING_API UGamePawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	UGamePawnManager();
	

	//////////////////////////////////////////////////////////////////////////
	/// Role Info
public:
	void LoadAllRolesInfo();
	TSoftClassPtr<AGamePawn> GetRoleClassFromName(const FName& Name) const;
	FName GetRoleNameFromClass(UClass* RoleClass) const;
	bool GetRoleInfoFromName(const FName& RoleName, const FRoleInfo* & OutRoleInfo) const;
	const TMap<FName, FRoleInfo>& GetAllRolesInfo() const;
private:
	TMap<FName, FRoleInfo> AllRolesInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Role Skin
public:
	void LoadAllRoleSkinInfo();
	TArray<FName> GetAllRoleSkinWithRoleName(const FName& RoleName);
	bool GetRoleSkinInfo(const FName& SkinName, const FRoleSkinInfo* & OutRoleSkinInfo) const;
private:
	TMap<FName, FRoleSkinInfo> AllRoleSkinInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Role Form
public:
	void LoadAllRoleFormInfo();
	TArray<FName> GetAllRoleFormWithRoleName(const FName& RoleName);
	bool GetRoleFormInfo(const FName& SkinName, const FRoleFormInfo* & OutRoleFormInfo) const;
private:
	TMap<FName, FRoleFormInfo> AllRoleFormInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Role Display Info
public:
	bool GetRoleDisplayInfo(const FName& RoleName, FRoleDisplayInfo const*& OutRoleDisplayInfo);
private:
	class UDataTable* RoleDisplayInfoDT;
	TMap<FName, FRoleDisplayInfo> AllRoleDisplayInfo;


	//////////////////////////////////////////////////////////////////////////
	/// Skin and Form class info
public:
	static FRoleSkin* CreateRoleSkin(const FName& Name, UStaticMeshComponent* InStaticMeshComp);
	static void RegisterRoleSkin(const FName& Name, FRoleSkinClassInfo* ClassInfo);

	static FRoleForm* CreateRoleForm(const FName& Name, AGamePawn* InGamePawn);
	static void RegisterRoleForm(const FName& Name, FRoleFormClassInfo* ClassInfo);

private:
	static TMap<FName, FRoleSkinClassInfo*> AllRoleSkinClassInfo;
	static TMap<FName, FRoleFormClassInfo*> AllRoleFormClassInfo;
};