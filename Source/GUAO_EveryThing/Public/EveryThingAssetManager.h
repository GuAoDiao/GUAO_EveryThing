// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"

#include "Blueprint/UserWidget.h"
#include "EveryThingTypes.h"

#include "EveryThingAssetManager.generated.h"

class UStaticMesh;
class UParticleSystem;
class UMaterialInstanceConstant;
class UGamePawnManager;

USTRUCT(BlueprintType)
struct FStaticMeshData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UStaticMesh> MeshClass;
};

USTRUCT(BlueprintType)
struct FMaterialInstanceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UMaterialInstanceConstant> MaterialInstanceClass;
};

USTRUCT(BlueprintType)
struct FParticleSystemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UParticleSystem> ParticleSystemClass;
};

USTRUCT(BlueprintType)
struct FUserWidgetData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UUserWidget> UserWidgetClass;
};

USTRUCT(BlueprintType)
struct FDataTableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UDataTable> DataTableClass;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GUAO_EVERYTHING_API UEveryThingAssetManager : public UObject
{
	GENERATED_BODY()
	
protected:
	UEveryThingAssetManager();

	virtual void FinishDestroy() override;
public:
	static UEveryThingAssetManager* GetAssetManagerInstance();
	static void DestroyAssetManagerInstance();
private:
	static UEveryThingAssetManager* AssetManager;
	
	FStreamableManager OwnerStreamableManager;
	//////////////////////////////////////////////////////////////////////////
	/// Mesh
public:
	void NeededMeshFromName(const FName& MeshName);
	UStaticMesh* GetMeshFromName(const FName& MeshName, bool bIsNeedForce = true);
private:
	void LoadMeshFromDT();
	TMap<FName, TSoftObjectPtr<UStaticMesh>> AllMeshAsset;


	//////////////////////////////////////////////////////////////////////////
	/// Material instance
public:
	void NeededMaterialFromName(const FName& MaterialName);
	UMaterialInstanceConstant* GetMaterialFromName(const FName& MaterialName, bool bIsNeedForce = false);

private:
	void LoadMaterialFromDT();
	TMap<FName, TSoftObjectPtr<UMaterialInstanceConstant>> AllMaterialInstanceAsset;

	//////////////////////////////////////////////////////////////////////////
	/// Particle
public:
	void NeededParticleFromName(const FName& ParticleName);
	UParticleSystem* GetParticleFromName(const FName& ParticleName, bool bIsNeedForce = false);

private:
	void LoadParticleFromDT();
	TMap<FName, TSoftObjectPtr<UParticleSystem>> AllParticleAsset;

	//////////////////////////////////////////////////////////////////////////
	/// User Widget
public:

	void LoadAllUserWidgetFromDT();
	TSubclassOf<UUserWidget> GetUserWidgetFromName(const FName& UserWidgetName);

private:	
	TMap<FName, TSoftClassPtr<UUserWidget>> AllUserWidgetAsset;

	//////////////////////////////////////////////////////////////////////////
	/// Data table

public:
	UDataTable* GetDataTableFromName(const FName& DataTableName);

private:
	TMap<FName, TSoftObjectPtr<UDataTable>> AllDataTableAsset;


	//////////////////////////////////////////////////////////////////////////
	/// Map Info
public:
	void LoadAllMapTypeAndMapInfoFromDT();
	const FMapInfo* GetMapInfoFromName(const FName& MapName);
	const FMapTypeInfo* GetMapTypeInfoFromName(const FName& MapName);
	const TArray<FString>& GetAllMapsType() { return MapsType; }
	TArray<FString> GetAllMapsFormType(const FString& MapType);
private:
	TArray<FString> MapsType;
	TMap<FName, FMapInfo> AllMapsInfo;
	TMap<FName, FMapTypeInfo> AllMapTypesInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Prop Info
public:
	void LoadAllPropInfoFromDT();
	const FPickupPropInfo* GetPropInfoFromID(int32 PropID) const;

private:
	TMap<int32, FPickupPropInfo> AllPropsInfo;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn
public:
	UGamePawnManager* GetGamePawnManager();
protected:
	UPROPERTY()
	UGamePawnManager* GamePawnManager;
};