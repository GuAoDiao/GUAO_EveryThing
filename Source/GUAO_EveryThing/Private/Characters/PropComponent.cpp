// Fill out your copyright notice in the Description page of Project Settings.

#include "PropComponent.h"

#include "UnrealNetwork.h"

#include "EveryThingAssetManager.h"
#include "Characters/GamePawn.h"
#include "SceneObject/Prop/PropBase.h"

UPropComponent::UPropComponent()
{
	bReplicates = true;

	MaxPropItemNum = 4;
	CurrentPropItemNum = 0;

	OwnerGamePawn = Cast<AGamePawn>(GetOwner());

	for (int32 i = 0; i < MaxPropItemNum; ++i)
	{
		AllPropsIndex.Add(-1);
	}
}



//////////////////////////////////////////////////////////////////////////
/// Game Prop
bool UPropComponent::AddProp(int32 PropID)
{
	for (int32 i = 0; i < AllProps.Num(); ++i)
	{
		if (AllProps[i].PropID == PropID)
		{
			++AllProps[i].PropNum;
			OnAllPropsChanged();
			return true;
		}
	}

	
	if (CurrentPropItemNum <= MaxPropItemNum)
	{
		FPropItemStroe StoreInfo;
		
		StoreInfo.PropID = PropID;
		StoreInfo.PropNum = 1;
				
		AllProps.Add(StoreInfo);

		++CurrentPropItemNum;

		for (int32 j = 0; j < MaxPropItemNum; ++j)
		{
			if (AllPropsIndex[j] == -1)
			{
				AllPropsIndex[j] = PropID;
				break;
			}
		}

		OnAllPropsIndexChanged();
		return true;
	}

	return false;
}

void UPropComponent::UsePropFromIndex(int32 Index)
{
	if (AllPropsIndex.IsValidIndex(Index))
	{
		UseProp(AllPropsIndex[Index]);
	}
}

void UPropComponent::UseProp(int32 PropID)
{
	for (int32 i = 0; i < AllProps.Num(); ++i)
	{
		if (AllProps[i].PropID == PropID)
		{
			const FPickupPropInfo* PropInfo = UEveryThingAssetManager::GetAssetManagerInstance()->GetPropInfoFromID(PropID);

			UPropBase* Prop = PropInfo && PropInfo->PropClass ? NewObject<UPropBase>(this, PropInfo->PropClass) : nullptr;
			if (Prop && Prop->BeUseByGamePawn(OwnerGamePawn))
			{
				--AllProps[i].PropNum;

				OnAllPropsChanged();

				if (AllProps[i].PropNum == 0)
				{
					--CurrentPropItemNum;
					AllProps.RemoveAt(i);

					for (int32 j = 0; j < MaxPropItemNum; ++j)
					{
						if (AllPropsIndex[j] == PropID)
						{
							AllPropsIndex[j] = -1;
							break;
						}
					}

					OnAllPropsIndexChanged();
				}
			}
		}
	}
}

int32 UPropComponent::GetPropNums(int32 PropID) const
{
	for (int32 i = 0; i < AllProps.Num(); ++i)
	{
		if (AllProps[i].PropID == PropID)
		{
			return AllProps[i].PropNum;
		}
	}
	return 0;
}


void UPropComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPropComponent, AllPropsIndex);
	DOREPLIFETIME(UPropComponent, AllProps);
}