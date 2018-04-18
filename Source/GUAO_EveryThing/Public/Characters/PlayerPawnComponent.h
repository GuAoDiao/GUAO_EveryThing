// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EveryThingTypes.h"
#include "PlayerPawnComponent.generated.h"

class IHitAbleInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUAO_EVERYTHING_API UPlayerPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerPawnComponent();
	
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginDestroy() override;

	void OnPossessedByController(class AController* NewController);

	void OnPlayerStateUpdate(class APlayerState* PlayerState);
	void OnPlayerInfoUpdate(const FPlayerInfo& InPlayerInfo);
	//////////////////////////////////////////////////////////////////////////
	/// For Input Component
	void RebindInputComp(class UInputComponent* OwnerInputComp);


	void StartTogglePawnForm();
	void StopTogglePawnForm();

	void StartTogglePawnSkin();
	void StopTogglePawnSkin();

	//////////////////////////////////////////////////////////////////////////
	/// Visual Angle
public:
	void Turn(float AxisValue);
	void LookUp(float AxisValue);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VisualAngle")
	float BaseTurnRate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VisualAngle")
	float BaseLookUpRate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VisualAngle")
	bool bIsConvertYAxis;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form And Prop Use
public:
	DECLARE_DELEGATE_OneParam(FOnPressNumberKeyboardDelegate, int32 /* NumberIndex */);

	void OnPressNumberKeyboard(int32 NumberIndex);

	FOnPressNumberKeyboardDelegate& GetOnPressNumberKeyboardDelegate() { return OnPressNumberKeyboardDelegate; }
private:
	FOnPressNumberKeyboardDelegate OnPressNumberKeyboardDelegate;

private:
	class AEveryThingHUD_Game* GetEveryThingGameHUD() const;

	void UseProp(int32 NumberIndex);
	void ToggleRoleForm(int32 NumberIndex);
	void ToggleRoleSkin(int32 NumberIndex);

	class AGamePawn* OwnerPawn;

	//////////////////////////////////////////////////////////////////////////
	/// Select Attack Target
public:
	AActor* GetAttackTarget() const { return CurrentAttackTarget; }
	
	void SelectNextAttackTarget();
	void SelectLastAttackTarget();

private:
	AActor* TryToGetHitAbleActor() const;
protected:
	UPROPERTY()
	AActor* LastAttackTarget;
	UPROPERTY()
	AActor* CurrentAttackTarget;
};