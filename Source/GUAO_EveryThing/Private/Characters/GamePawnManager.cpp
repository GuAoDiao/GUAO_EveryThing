// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnManager.h"

#include "Characters/GamePawn.h"
#include "EveryThingAssetManager.h"
#include "Characters/Skin/GamePawnSkin.h"
#include "Characters/Form/GamePawnForm.h"


TMap<FName, FRoleSkinClassInfo*> UGamePawnManager::AllRoleSkinClassInfo;
TMap<FName, FRoleFormClassInfo*> UGamePawnManager::AllRoleFormClassInfo;

UGamePawnManager::UGamePawnManager()
{
	LoadAllRolesInfo();
	LoadAllRoleSkinInfo();
	LoadAllRoleFormInfo();

	RoleDisplayInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RoleDisplayInfo"));
}

//////////////////////////////////////////////////////////////////////////
/// Load
void UGamePawnManager::LoadAllRolesInfo()
{
	UDataTable* RolesInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RolesInfo"));
	if (RolesInfoDT)
	{
		TArray<FRoleInfo*> RolesInfoInDatatable;
		RolesInfoDT->GetAllRows<FRoleInfo>(TEXT("found all Roles Name in DataTable"), RolesInfoInDatatable);
		for (FRoleInfo* RoleInfo : RolesInfoInDatatable)
		{
			AllRolesInfo.Add(RoleInfo->Name, *RoleInfo);
		}
	}
}

void UGamePawnManager::LoadAllRoleSkinInfo()
{
	UDataTable* RoleSkinInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RoleSkinInfo"));
	if (RoleSkinInfoDT)
	{
		TArray<FRoleSkinInfo*> RoleSkinInfoInDatatable;
		RoleSkinInfoDT->GetAllRows<FRoleSkinInfo>(TEXT("found all Roles Name in DataTable"), RoleSkinInfoInDatatable);
		for (FRoleSkinInfo* SkinInfo : RoleSkinInfoInDatatable)
		{
			AllRoleSkinInfo.Add(SkinInfo->Name, *SkinInfo);
		}
	}
}

void UGamePawnManager::LoadAllRoleFormInfo()
{

	UDataTable* RoleFormInfoDT = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName(TEXT("RoleFormInfo"));
	if (RoleFormInfoDT)
	{
		TArray<FRoleFormInfo*> RoleFormInfoInDatatable;
		RoleFormInfoDT->GetAllRows<FRoleFormInfo>(TEXT("found all Roles Name in DataTable"), RoleFormInfoInDatatable);
		for (FRoleFormInfo* RoleFormInfo : RoleFormInfoInDatatable)
		{
			AllRoleFormInfo.Add(RoleFormInfo->Name, *RoleFormInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Role Info
TSoftClassPtr<AGamePawn> UGamePawnManager::GetRoleClassFromName(const FName& RolesName) const
{
	if (AllRolesInfo.Contains(RolesName))
	{
		return AllRolesInfo[RolesName].RoleClass;
	}

	return nullptr;
}

FName UGamePawnManager::GetRoleNameFromClass(UClass* RoleClass) const
{
	if (RoleClass)
	{
		for (TMap<FName, FRoleInfo>::TConstIterator It(AllRolesInfo); It; ++It)
		{
			UClass* CurrentClass = It.Value().RoleClass.Get();
			if (CurrentClass->IsChildOf(RoleClass) || RoleClass->IsChildOf(CurrentClass))
			{
				return It.Value().Name;
			}
		}
	}

	return NAME_None;
}

bool UGamePawnManager::GetRoleInfoFromName(const FName& RoleName, const FRoleInfo* & OutRoleInfo) const
{
	if (AllRolesInfo.Contains(RoleName))
	{
		OutRoleInfo = &AllRolesInfo[RoleName];
		return true;
	}

	return false;
}

const TMap<FName, FRoleInfo>& UGamePawnManager::GetAllRolesInfo() const
{
	return AllRolesInfo;
}

//////////////////////////////////////////////////////////////////////////
/// Role Skin Info

TArray<FName> UGamePawnManager::GetAllRoleSkinWithRoleName(const FName& RoleName)
{
	TArray<FName> Result;
	for (TMap<FName, FRoleSkinInfo>::TConstIterator It(AllRoleSkinInfo); It; ++It)
	{
		if (It.Value().RoleName == RoleName)
		{
			Result.AddUnique(It.Key());
		}
	}
	return Result;
}

bool UGamePawnManager::GetRoleSkinInfo(const FName& SkinName, const FRoleSkinInfo* & OutRoleSkinInfo) const
{
	if (AllRoleSkinInfo.Contains(SkinName))
	{
		OutRoleSkinInfo = &AllRoleSkinInfo[SkinName];
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
/// Role Form Info
bool UGamePawnManager::GetRoleFormInfo(const FName& FormName, const FRoleFormInfo* & OutRoleFormInfo) const
{
	if (AllRoleFormInfo.Contains(FormName))
	{
		OutRoleFormInfo = &AllRoleFormInfo[FormName];
		return true;
	}
	return false;
}



TArray<FName> UGamePawnManager::GetAllRoleFormWithRoleName(const FName& RoleName)
{
	TArray<FName> Result;
	for (TMap<FName, FRoleFormInfo>::TConstIterator It(AllRoleFormInfo); It; ++It)
	{
		if (It.Value().RoleName == RoleName)
		{
			Result.AddUnique(It.Key());
		}
	}
	return Result;
}




//////////////////////////////////////////////////////////////////////////
/// Role Display Info

bool UGamePawnManager::GetRoleDisplayInfo(const FName& RoleName, FRoleDisplayInfo const*& OutRoleDisplayInfo)
{
	if (AllRoleDisplayInfo.Contains(RoleName))
	{
		OutRoleDisplayInfo = &AllRoleDisplayInfo[RoleName];
		return true;
	}


	if (RoleDisplayInfoDT)
	{
		FRoleDisplayInfo* RoleDisplayInfo = RoleDisplayInfoDT->FindRow<FRoleDisplayInfo>(RoleName, TEXT("-_- find role display info"));
		if (RoleDisplayInfo)
		{
			AllRoleDisplayInfo.Add(RoleName, *RoleDisplayInfo);
			OutRoleDisplayInfo = &AllRoleDisplayInfo[RoleName];
			return true;
		}
	}

	return false;
}





//////////////////////////////////////////////////////////////////////////
/// Skin and Form class info

FRoleSkin* UGamePawnManager::CreateRoleSkin(const FName& Name, UStaticMeshComponent* InStaticMeshComp)
{
	if (AllRoleSkinClassInfo.Contains(Name))
	{
		return AllRoleSkinClassInfo[Name]->CreateObject(InStaticMeshComp);
	}
	return nullptr;
}

void UGamePawnManager::RegisterRoleSkin(const FName& Name, FRoleSkinClassInfo* ClassInfo)
{
	AllRoleSkinClassInfo.Add(Name, ClassInfo);
}



FRoleForm* UGamePawnManager::CreateRoleForm(const FName& Name, AGamePawn* InGamePawn)
{
	if (AllRoleFormClassInfo.Contains(Name))
	{
		return AllRoleFormClassInfo[Name]->CreateObject(InGamePawn);
	}
	return nullptr;
}

void UGamePawnManager::RegisterRoleForm(const FName& Name, FRoleFormClassInfo* ClassInfo)
{
	AllRoleFormClassInfo.Add(Name, ClassInfo);
}