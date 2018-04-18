// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawnForm.h"

#include "Characters/GamePawn.h"
#include "Characters/Moves/SkillComponent.h"
#include "Characters/Moves/AttackComponent.h"

FRoleForm::FRoleForm(AGamePawn* InGamePawn)
{
	OwnerGamePawn = InGamePawn;
}

void FRoleForm::LoadGamePawnForm()
{

	if (OwnerGamePawn && OwnerGamePawn->HasAuthority())
	{

		if (!OwnerGamePawn->IsActorInitialized())
		{
			UObject* CreateDefaultSubobject(FName SubobjectFName, UClass* ReturnType, UClass* ClassToCreateByDefault, bool bIsRequired, bool bAbstract, bool bIsTransient);

			OwnerGamePawn->ToggleToNewAttackComponent(Cast<UAttackComponent>(OwnerGamePawn->CreateDefaultSubobject(TEXT("AttackComp"), UAttackComponent::StaticClass(), AttackCompClass, true, false, false)));
			OwnerGamePawn->ToggleToNewSkillComponent(Cast<USkillComponent>(OwnerGamePawn->CreateDefaultSubobject(TEXT("SkillComp"), USkillComponent::StaticClass(), SkillCompClass, true, false, false)));
		}
		else
		{
			UAttackComponent* AttackComp = NewObject<UAttackComponent>(OwnerGamePawn, AttackCompClass);
			if (AttackComp) { AttackComp->RegisterComponent(); }

			OwnerGamePawn->ToggleToNewAttackComponent(AttackComp);

			USkillComponent* SkillComp = NewObject<USkillComponent>(OwnerGamePawn, SkillCompClass);
			if (SkillComp) { SkillComp->RegisterComponent(); }
			
			OwnerGamePawn->ToggleToNewSkillComponent(SkillComp);
		}
	}
}

void FRoleForm::UnloadGamePawnForm()
{
	if (OwnerGamePawn)
	{
		if (OwnerGamePawn->GetAttackComponent()) { OwnerGamePawn->GetAttackComponent()->DestroyComponent(); }
		if (OwnerGamePawn->GetSkillComponent()) { OwnerGamePawn->GetSkillComponent()->DestroyComponent(); }
	}
}