// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PropComponent.generated.h"


USTRUCT(BlueprintType)
struct FPropItemStroe
{
	GENERATED_USTRUCT_BODY()
public:
	FPropItemStroe() : PropID(-1), PropNum(0) {}

	UPROPERTY(BlueprintReadWrite)
	int32 PropID;
	UPROPERTY(BlueprintReadWrite)
	int32 PropNum;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UPropComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPropComponent();

protected:


	//////////////////////////////////////////////////////////////////////////
	/// Game Prop
public:
	bool AddProp(int32 PropID);
	void UsePropFromIndex(int32 Index);
	void UseProp(int32 PropID);
	int32 GetPropNums(int32 PropID) const;
	int32 GetMaxPropItemNum() const { return MaxPropItemNum; }
	const TArray<int32>& GetAllPropsIndex() const { return AllPropsIndex; }

protected:
	class AGamePawn* OwnerGamePawn;

	int32 CurrentPropItemNum;
	int32 MaxPropItemNum;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllPropsIndexChangedDelegate, const TArray<int32>& /*AllPropsIndex*/)
	FOnAllPropsIndexChangedDelegate OnAllPropsIndexChangedDelegate;


	DECLARE_MULTICAST_DELEGATE(FOnAllPropsChangedDelegate)
	FOnAllPropsChangedDelegate OnAllPropsChangedDelegate;
protected:
	UFUNCTION()
	void OnRep_AllPropsIndex() { OnAllPropsIndexChanged(); }
	void OnAllPropsIndexChanged() { OnAllPropsIndexChangedDelegate.Broadcast(AllPropsIndex); }
	UFUNCTION()
	void OnRep_AllProps() { OnAllPropsChanged(); }
	void OnAllPropsChanged() { OnAllPropsChangedDelegate.Broadcast();}

	UPROPERTY(ReplicatedUsing = OnRep_AllPropsIndex)
	TArray<int32> AllPropsIndex;
	UPROPERTY(ReplicatedUsing = OnRep_AllProps)
	TArray<FPropItemStroe> AllProps;
};
