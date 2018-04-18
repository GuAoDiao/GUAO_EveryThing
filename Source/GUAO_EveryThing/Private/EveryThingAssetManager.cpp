// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryThingAssetManager.h"

#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystem.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Characters/GamePawnManager.h"

UEveryThingAssetManager* UEveryThingAssetManager::AssetManager = nullptr;

UEveryThingAssetManager::UEveryThingAssetManager()
{
#if !WITH_EDITORONLY_DATA
	SetFlags(RF_Standalone);
#endif

	AssetManager = this;

	// load all DataTable
	UDataTable* AllDataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/EveryThing/DataTable/DT_DataTable.DT_DataTable'"));
	if (AllDataTable)
	{
		TArray<FDataTableData*> AllDataTableInfo;
		AllDataTable->GetAllRows<FDataTableData>(TEXT("found all DataTable"), AllDataTableInfo);
		for (FDataTableData* DataTable : AllDataTableInfo)
		{
			AllDataTableAsset.Add(DataTable->Name, DataTable->DataTableClass);
		}
	}

	// load all info
	LoadMeshFromDT();
	LoadMaterialFromDT();
	LoadParticleFromDT();
	LoadAllUserWidgetFromDT();
	LoadAllPropInfoFromDT();
	LoadAllMapTypeAndMapInfoFromDT();
}

void UEveryThingAssetManager::FinishDestroy()
{
	AssetManager = nullptr;

	UE_LOG(LogTemp, Log, TEXT("-_- destory everything asset manager."));

	Super::FinishDestroy();
}

UEveryThingAssetManager* UEveryThingAssetManager::GetAssetManagerInstance()
{
	if (!AssetManager)
	{
		AssetManager = NewObject<UEveryThingAssetManager>();
	}

	checkf(AssetManager, TEXT("The Asset Manager must be exists."));

	return AssetManager;
}

void UEveryThingAssetManager::DestroyAssetManagerInstance()
{
	AssetManager = nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// Mesh
void UEveryThingAssetManager::LoadMeshFromDT()
{
	UDataTable* StaticMeshDatatable = GetDataTableFromName(TEXT("StaticMesh"));
	if (StaticMeshDatatable)
	{
		TArray<FStaticMeshData*> StaticMeshDataInDatatable;
		StaticMeshDatatable->GetAllRows<FStaticMeshData>(TEXT("found all StaticMesh DataTable"), StaticMeshDataInDatatable);
		for (FStaticMeshData* StaticMeshData : StaticMeshDataInDatatable)
		{
			AllMeshAsset.Add(StaticMeshData->Name, StaticMeshData->MeshClass);
		}
	}
}

void UEveryThingAssetManager::NeededMeshFromName(const FName& MeshName) { GetMeshFromName(MeshName, false); }
UStaticMesh* UEveryThingAssetManager::GetMeshFromName(const FName& MeshName, bool bIsNeedForce)
{
	if (!AllMeshAsset.Contains(MeshName))
	{
		AllMeshAsset.Add(MeshName, TSoftObjectPtr<UStaticMesh>(FString::Printf(TEXT("StaticMesh'/Game/EveryThing/Meshes/SM_%s.SM_%s'"), *MeshName.ToString(), *MeshName.ToString())));
	}

	if (bIsNeedForce)
	{
		return AllMeshAsset[MeshName].LoadSynchronous();
	}
	else
	{
		UStaticMesh* Result = AllMeshAsset[MeshName].Get();
		if (!Result) { OwnerStreamableManager.RequestAsyncLoad(AllMeshAsset[MeshName].ToSoftObjectPath()); }
		return Result;
	}
}


//////////////////////////////////////////////////////////////////////////
/// Material instance

void UEveryThingAssetManager::LoadMaterialFromDT()
{
	UDataTable* MaterialInstaneDatatable = GetDataTableFromName(TEXT("MaterialInstance"));
	if (MaterialInstaneDatatable)
	{
		TArray<FMaterialInstanceData*> MaterialInstanceDataInDatatable;
		MaterialInstaneDatatable->GetAllRows<FMaterialInstanceData>(TEXT("found all MaterialInstane DataTable"), MaterialInstanceDataInDatatable);
		for (FMaterialInstanceData* MaterialInstaneData : MaterialInstanceDataInDatatable)
		{
			AllMaterialInstanceAsset.Add(MaterialInstaneData->Name, MaterialInstaneData->MaterialInstanceClass);
		}
	}
}

void UEveryThingAssetManager::NeededMaterialFromName(const FName& MaterialName) { GetMaterialFromName(MaterialName, false); }
UMaterialInstanceConstant* UEveryThingAssetManager::GetMaterialFromName(const FName& MaterialName, bool bIsNeedForce)
{
	if (!AllMaterialInstanceAsset.Contains(MaterialName))
	{
		AllMaterialInstanceAsset.Add(MaterialName, TSoftObjectPtr<UMaterialInstanceConstant>(FString::Printf(TEXT("MaterialInstanceConstant'/Game/EveryThing/Materials/MI_%s.MI_%s'"), *MaterialName.ToString(), *MaterialName.ToString())));
	}

	if (bIsNeedForce)
	{
		return AllMaterialInstanceAsset[MaterialName].LoadSynchronous();
	}
	else
	{
		UMaterialInstanceConstant* Result = AllMaterialInstanceAsset[MaterialName].Get();
		if (!Result) { OwnerStreamableManager.RequestAsyncLoad(AllMaterialInstanceAsset[MaterialName].ToSoftObjectPath()); }
		return Result;
	}
}



//////////////////////////////////////////////////////////////////////////
/// Particle

void UEveryThingAssetManager::LoadParticleFromDT()
{
	UDataTable* ParticleSystemDatatable = GetDataTableFromName(TEXT("ParticleSystem"));
	if (ParticleSystemDatatable)
	{
		TArray<FParticleSystemData*> ParticleSystemDataInDatatable;
		ParticleSystemDatatable->GetAllRows<FParticleSystemData>(TEXT("found all ParticleSystem DataTable"), ParticleSystemDataInDatatable);
		for (FParticleSystemData* ParticleSystemData : ParticleSystemDataInDatatable)
		{
			AllParticleAsset.Add(ParticleSystemData->Name, ParticleSystemData->ParticleSystemClass);
		}
	}
}

void UEveryThingAssetManager::NeededParticleFromName(const FName& ParticleName) { GetParticleFromName(ParticleName, false); }
UParticleSystem* UEveryThingAssetManager::GetParticleFromName(const FName& ParticleName, bool bIsNeedForce)
{
	if (!AllParticleAsset.Contains(ParticleName))
	{
		AllParticleAsset.Add(ParticleName, TSoftObjectPtr<UParticleSystem>(FString::Printf(TEXT("ParticleSystem'/Game/EveryThing/Particles/P_%s.P_%s'"), *ParticleName.ToString(), *ParticleName.ToString())));
	}

	if (bIsNeedForce)
	{
		return AllParticleAsset[ParticleName].LoadSynchronous();
	}
	else
	{
		UParticleSystem* Result = AllParticleAsset[ParticleName].Get();
		if (!Result) { OwnerStreamableManager.RequestAsyncLoad(AllParticleAsset[ParticleName].ToSoftObjectPath()); }
		return Result;
	}
}



//////////////////////////////////////////////////////////////////////////
/// User Widget

void UEveryThingAssetManager::LoadAllUserWidgetFromDT()
{
	UDataTable* UserWidgetDatatable = GetDataTableFromName(TEXT("UserWidget"));
	if (UserWidgetDatatable)
	{
		TArray<FUserWidgetData*> UserWidgetDataInDatatable;
		UserWidgetDatatable->GetAllRows<FUserWidgetData>(TEXT("found all UserWidget in DataTable"), UserWidgetDataInDatatable);
		for (FUserWidgetData* UserWidgetData : UserWidgetDataInDatatable)
		{
			AllUserWidgetAsset.Add(UserWidgetData->Name, UserWidgetData->UserWidgetClass);
		}
	}
}


TSubclassOf<UUserWidget> UEveryThingAssetManager::GetUserWidgetFromName(const FName& UserWidgetName)
{
	if (!AllUserWidgetAsset.Contains(UserWidgetName))
	{
		AllUserWidgetAsset.Add(UserWidgetName, TSoftClassPtr<UUserWidget>(FString::Printf(TEXT("WidgetBlueprint'/Game/EveryThing/UI/W_%s.W_%s_C'"), *UserWidgetName.ToString(), *UserWidgetName.ToString())));
	}
	return AllUserWidgetAsset[UserWidgetName].LoadSynchronous();
}

//////////////////////////////////////////////////////////////////////////
/// Data table
UDataTable* UEveryThingAssetManager::GetDataTableFromName(const FName& DataTableName)
{
	if (!AllDataTableAsset.Contains(DataTableName))
	{
		AllDataTableAsset.Add(DataTableName, TSoftObjectPtr<UDataTable>(FString::Printf(TEXT("DataTable'/Game/EveryThing/DataTable/DT_%s.DT_%s'"), *DataTableName.ToString(), *DataTableName.ToString())));
	}
	return AllDataTableAsset[DataTableName].LoadSynchronous();
}


//////////////////////////////////////////////////////////////////////////
/// Map Info

void UEveryThingAssetManager::LoadAllMapTypeAndMapInfoFromDT()
{
	UDataTable* MapTypeInfoDataTable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName("MapTypesInfo");
	if (MapTypeInfoDataTable)
	{
		TArray<FMapTypeInfo*> MapsTypeInfoInDataTable;
		MapTypeInfoDataTable->GetAllRows<FMapTypeInfo>(TEXT("look up all maps info"), MapsTypeInfoInDataTable);
		for (FMapTypeInfo* MapTypeInfo : MapsTypeInfoInDataTable)
		{
			MapsType.AddUnique(MapTypeInfo->Type);
			AllMapTypesInfo.Add(FName(*MapTypeInfo->Type), *MapTypeInfo);
		}
	}

	UDataTable* MapsInfoDataTable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName("MapsInfo");
	if (MapsInfoDataTable)
	{
		TArray<FMapInfo*> MapsInfoInDataTable;
		MapsInfoDataTable->GetAllRows<FMapInfo>(TEXT("look up all maps info"), MapsInfoInDataTable);
		for (FMapInfo* MapInfo : MapsInfoInDataTable)
		{
			if (AllMapTypesInfo.Contains(FName(*MapInfo->MapType)))
			{
				AllMapsInfo.Add(FName(*MapInfo->MapName), *MapInfo);
			}
		}
	}
}

TArray<FString> UEveryThingAssetManager::GetAllMapsFormType(const FString& MapType)
{
	TArray<FString> Result;
	for (TMap<FName, FMapInfo>::TConstIterator It(AllMapsInfo); It; ++It)
	{
		if (It.Value().MapType == MapType)
		{
			Result.AddUnique(It.Value().MapName);
		}
	}
	return Result;
}

const FMapInfo* UEveryThingAssetManager::GetMapInfoFromName(const FName& MapName)
{
	if (AllMapsInfo.Contains(MapName))
	{
		return &AllMapsInfo[MapName];
	}
	return nullptr;
}

const FMapTypeInfo* UEveryThingAssetManager::GetMapTypeInfoFromName(const FName& MapName)
{
	if (AllMapTypesInfo.Contains(MapName))
	{
		return &AllMapTypesInfo[MapName];
	}
	return nullptr;
}


//////////////////////////////////////////////////////////////////////////
/// Prop Info
void UEveryThingAssetManager::LoadAllPropInfoFromDT()
{
	UDataTable* PropInfoDatatable = GetDataTableFromName(TEXT("PropInfo"));
	if (PropInfoDatatable)
	{
		TArray<FPickupPropInfo*> PropInfoInDatatable;
		PropInfoDatatable->GetAllRows<FPickupPropInfo>(TEXT("found all PropInfo in DataTable"), PropInfoInDatatable);
		for (FPickupPropInfo* PickupPropInfo : PropInfoInDatatable)
		{
			AllPropsInfo.Add(PickupPropInfo->ID, *PickupPropInfo);
		}
	}
}
const FPickupPropInfo* UEveryThingAssetManager::GetPropInfoFromID(int32 PropID) const
{
	if (AllPropsInfo.Contains(PropID))
	{
		return &AllPropsInfo[PropID];
	}
	return nullptr;
}


//////////////////////////////////////////////////////////////////////////
/// Game Pawn
UGamePawnManager* UEveryThingAssetManager::GetGamePawnManager()
{
	if (!GamePawnManager)
	{
		GamePawnManager = NewObject<UGamePawnManager>(this);
	}

	checkf(GamePawnManager, TEXT("The GamePawnManager must exists"));

	return GamePawnManager;
}