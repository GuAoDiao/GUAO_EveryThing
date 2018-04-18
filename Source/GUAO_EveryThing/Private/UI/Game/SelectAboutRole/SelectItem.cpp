// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectItem.h"



void USelectItem::InitializeSelectItem_Implementation(const FName& InItemName, bool bHave)
{
	ItemName = InItemName;
	UpdateSelectItemDisplay(bHave);
}
