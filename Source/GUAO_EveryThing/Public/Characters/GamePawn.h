// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "EveryThingTypes.h"
#include "SceneObject/HitAbleInterface.h"

#include "GamePawn.generated.h"

class UAttackComponent;
class USkillComponent;
class FRoleForm;
class FRoleSkin;
class UGamePawnMovementComponent;
class UPropComponent;

UCLASS()
class GUAO_EVERYTHING_API AGamePawn : public APawn, public IHitAbleInterface
{
	GENERATED_BODY()

public:
	AGamePawn();

	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	FName RoleName;

	//////////////////////////////////////////////////////////////////////////
	/// Possessed By

	virtual void PossessedBy(AController* NewController) override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPossessedByControllerDelegate, AController* /* NewController */);
	FOnPossessedByControllerDelegate OnPossessedByControllerDelegate;

	//////////////////////////////////////////////////////////////////////////
	/// Component
public:
	inline UGamePawnMovementComponent* GetGamePawnMovementComponent() const { return MovementComp; }
	inline UPropComponent* GetPropComponent() const { return PropComp; }
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UGamePawnMovementComponent* MovementComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UPropComponent* PropComp;

	//////////////////////////////////////////////////////////////////////////
	/// HitAble Interface
public:
	virtual void SetIsSelectedToHit(bool bInIsSelectedToHit) override;
	virtual void AcceptHitFrom(AActor* OtherActor, FVector NormalInpulse, const FHitResult& Hit) override;
	virtual const FString& GetHitAbleActorDisplayName() const override { return HitAbleDisplayName; }
	virtual bool CanBeSelectedToHit(AActor* Selector) const override;
	void SetHitAbleDisplayName(const FString& InHitAbleDisplayName) { HitAbleDisplayName = InHitAbleDisplayName; }
protected:
	UPROPERTY(Replicated)
	FString HitAbleDisplayName;
	//////////////////////////////////////////////////////////////////////////
	/// Hit
public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit);
protected:
	virtual void OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit);

public:
	DECLARE_MULTICAST_DELEGATE_FiveParams(FOnHitDelegate, UPrimitiveComponent* /* HitComp */, AActor* /* OtherActor */, UPrimitiveComponent* /* OtherComp */, FVector /* NormalInpulse */, const FHitResult& /* Hit */);
	FOnHitDelegate OnHitDelegate;


	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form And Skin
protected:
	void ResetDefaultSkinAndFormFromDataTable();

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Form
public:
	void ToggleToNewFormWithIndex(int32 Index);

	TArray<FName> AllRoleFormName;
	TArray<FName> AllHaveRoleFormName;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllHaveRoleFormNameUpdateDelegate, const TArray<FName>& /* AllHaveRoleSkinName */);
	FOnAllHaveRoleFormNameUpdateDelegate OnAllHaveRoleFormNameUpdateDelegate;

	void OnAllHaveRoleFormNameUpdate() { OnAllHaveRoleFormNameUpdateDelegate.Broadcast(AllHaveRoleFormName); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetFormSuccessDelegate, const FName& /* TargetFormName */);
	FOnToggleToTargetFormSuccessDelegate OnToggleToTargetFormSuccessDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetFormFailureDelegate, const FName& /* TargetFormName */, const FText&  /* ErrorInfo */);
	FOnToggleToTargetFormFailureDelegate OnToggleToTargetFormFailureDelegate;

protected:
	// actual implementation
	void ToggleToTargetForm(const FName& FormName);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToTargetForm(const FName& FormName);
	UFUNCTION()
	void OnRep_CurrentRoleFormName();
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentRoleFormName)
	FName CurrentRoleFormName;
	FRoleForm* CurrentRoleForm;

	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn Skin
public:
	void ToggleToNewSkinWithIndex(int32 Index);

	TArray<FName> AllRoleSkinName;
	TArray<FName> AllHaveRoleSkinName;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllHaveRoleSkinNameUpdateDelegate, const TArray<FName>& /* AllHaveRoleSkinName */);
	FOnAllHaveRoleSkinNameUpdateDelegate OnAllHaveRoleSkinNameUpdateDelegate;
	void OnAllHaveRoleSkinNameUpdate() { OnAllHaveRoleSkinNameUpdateDelegate.Broadcast(AllHaveRoleSkinName); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleToTargetSkinSuccessDelegate, const FName& /* TargetSkinName */);
	FOnToggleToTargetSkinSuccessDelegate OnToggleToTargetSkinSuccessDelegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleToTargetSkinFailureDelegate, const FName& /* TargetSkinName */, const FText&  /* ErrorInfo */);
	FOnToggleToTargetSkinFailureDelegate OnToggleToTargetSkinFailureDelegate;

protected:
	void ToggleToTargetSkin(const FName& SkinName);
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleToTargetSkin(const FName& SkinName);
	// actual implementation
	UFUNCTION()
	void OnRep_CurrentRoleSkinName();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentRoleSkinName)
	FName CurrentRoleSkinName;
	FRoleSkin* CurrentRoleSkin;

	//////////////////////////////////////////////////////////////////////////
	/// Attack and Skill
public:
	AActor* TryToGetAttackTarget(float InMaxAttackDistance);

	inline UAttackComponent* GetAttackComponent() const { return OwnerAttackComp; }
	inline USkillComponent* GetSkillComponent() const { return OwnerSkillComp; }

	void ToggleToNewAttackComponent(UAttackComponent* InAttackComponent);
	void ToggleToNewSkillComponent(USkillComponent* InSkillComponent);
protected:
	UFUNCTION()
	void OnRep_OwnerAttackComp();
	UFUNCTION()
	void OnRep_OwnerSkillComp();
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Replicated, ReplicatedUsing = OnRep_OwnerAttackComp)
	UAttackComponent* OwnerAttackComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Replicated, ReplicatedUsing = OnRep_OwnerSkillComp)
	USkillComponent* OwnerSkillComp;


	//////////////////////////////////////////////////////////////////////////
	/// Game Pawn info
public:
	float GetMaxHyperopiaDistance() const { return BaseInfo.MaxHyperopiaDistance; }
protected:
	void ResetInfoFromDataTable(const FName& GamePawnName);
	void SetInfo(const FGamePawnInfo* InInfo);

	UPROPERTY(BlueprintReadOnly, Replicated)
	FGamePawnInfo BaseInfo;
	

	//////////////////////////////////////////////////////////////////////////
	/// Durability
public:
	UFUNCTION(BlueprintPure)
	float GetDurability() const { return Durability; }
	UFUNCTION(BlueprintPure)
	float GetMaxDurability() const { return MaxDurability; }

	void ChangeDurability(float DurabilityOffset);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDurabilityUpdateDelegate, float /* Durability */);
	FOnDurabilityUpdateDelegate OnDurabilityUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxDurabilityUpdateDelegate, float /* MaxDurability */);
	FOnMaxDurabilityUpdateDelegate OnMaxDurabilityUpdateDelegate;
protected:

	UFUNCTION()
	void OnRep_Durability() { OnDurabilityUpdate(); }
	void OnDurabilityUpdate() { OnDurabilityUpdateDelegate.Broadcast(Durability); }

	UFUNCTION()
	void OnRep_MaxDurability() { OnMaxDurabilityUpdate(); }
	void OnMaxDurabilityUpdate() { OnMaxDurabilityUpdateDelegate.Broadcast(MaxDurability); }

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Durability)
	float Durability;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxDurability)
	float MaxDurability;

	//////////////////////////////////////////////////////////////////////////
	/// Stamina
public:
	UFUNCTION(BlueprintPure)
	float GetStamina() const { return Stamina; }
	UFUNCTION(BlueprintPure)
	float GetMaxStamina() const { return MaxStamina; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaUpdateDelegate, float /* Stamina */);
	FOnStaminaUpdateDelegate OnStaminaUpdateDelegate;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaUpdateDelegate, float /* MaxStamina */);
	FOnStaminaUpdateDelegate OnMaxStaminaUpdateDelegate;
protected:
	void SpendStamina(float Value);
	void ChangeStaminaTick(float DeltaTime);

	UFUNCTION()
	void OnRep_Stamina() { OnStaminaUpdate(); }
	void OnStaminaUpdate() { OnStaminaUpdateDelegate.Broadcast(Stamina); }
	UFUNCTION()
	void OnRep_MaxStamina() { OnMaxStaminaUpdate(); }
	void OnMaxStaminaUpdate() { OnMaxStaminaUpdateDelegate.Broadcast(MaxStamina); }

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina)
	float Stamina;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	float MaxStamina;
	UPROPERTY(BlueprintReadOnly)
	float StaminaRecoverRate;

	//////////////////////////////////////////////////////////////////////////
	/// Cure, Damage, Death

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	void Healed(AActor* Curer, float Treatment);
	void GamePawnDeath();
	void DelayToDestroy();
protected:
	bool bIsDeath;
	float DissolvePercent;
	AActor* LastDamageCauserActor;
	FTimerHandle DelayToDestroyTimer;

	//////////////////////////////////////////////////////////////////////////
	/// Element

protected:

	//////////////////////////////////////////////////////////////////////////
	/// Quality And Damping
public:
	float GetAgility() const { return Agility; }
	void SetAgility(float InAgility) { Agility = InAgility; OnAgilityAndQualityChanged(); }
	void SetQualityScale(float InQualityScale) { QualityScale = InQualityScale; OnAgilityAndQualityChanged(); }
protected:
	UPROPERTY(ReplicatedUsing = OnRep_AgilityAndQuality)
	float Agility;
	UPROPERTY(ReplicatedUsing = OnRep_AgilityAndQuality)
	float Quality;
	UPROPERTY(ReplicatedUsing = OnRep_AgilityAndQuality)
	float QualityScale;

	UFUNCTION()
	void OnRep_AgilityAndQuality() { OnAgilityAndQualityChanged(); }
	void OnAgilityAndQualityChanged();

	void ResetQuality();
	void ResetDamping();


	//////////////////////////////////////////////////////////////////////////
	/// On Use Force 
public:
	void ChangeCansumeScale(int32 InCansumeScale) { CansumeScale = InCansumeScale; }
	int32 GetCansumeScale() const { return CansumeScale; }

	void OnConsumeForce(const FVector& Force);
	void OnConsumeTorqueInRadians(const FVector& Torque);
	void OnConsumeImpulse(const FVector& Impulse);

	bool CanConsumeForce(const FVector& Force);
	bool CanConsumeTorqueInRadians(const FVector& Torque);
	bool CanConsumeImpulse(const FVector& Impulse);

protected:
	inline int32 GetConsumeForceValue(const FVector& Force) const { return Force.Size() * ConsumeForceScale * CansumeScale; }
	inline int32 GetConsumeTorqueInRadiansValue(const FVector& Torque) const { return Torque.Size() * ConsumeTorqueScale * CansumeScale; }
	inline int32 GetConsumeImpulseValue(const FVector& Impulse) const { return Impulse.Size() * ConsumeImpluseScale * CansumeScale; }

	float CansumeScale;
	float ConsumeForceScale;
	float ConsumeTorqueScale;
	float ConsumeImpluseScale;
};