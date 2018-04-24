// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Moves/MovesComponent.h"
#include "AttackComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UAttackComponent : public UMovesComponent
{
	GENERATED_BODY()

public:
	UAttackComponent();

	virtual void RebindInputComp(class UInputComponent* OwnerInputComp) override;

	inline FGamePawnMoves* GetCommantAttackSkilledness() { return &CommonAttack; }
	inline FGamePawnMoves* GetSpecialAttackSkilledness() { return &SpecialAttack; }
protected:
	virtual void StartCommonAttack();
	virtual void StopCommonAttack();
	virtual void ExcuteCommonAttack(float AxisValue);

	virtual void StartSpecialAttack();
	virtual void StopSpecialAttack();
	virtual void ExcuteSpecialAttack(float AxisValue);

	virtual void RebindAll() override;

protected:
	FGamePawnMoves CommonAttack;
	FGamePawnMoves SpecialAttack;
};
