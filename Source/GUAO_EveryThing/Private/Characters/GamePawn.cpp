// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePawn.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


#include "EveryThingAssetManager.h"
#include "Online/EveryThingGameMode_Game.h"
#include "Characters/PropComponent.h"
#include "Characters/Form/GamePawnForm.h"
#include "Characters/Skin/GamePawnSkin.h"
#include "Characters/Moves/AttackComponent.h"
#include "Characters/Moves/SkillComponent.h"
#include "Online/PlayerController_Game.h"
#include "Characters/Movement/Components/GamePawnMovementComponent.h"
#include "EveryThingGameState_Game.h"

#define LOCTEXT_NAMESPACE "Everything_Characters_GamePawn"

AGamePawn::AGamePawn()
{
	bReplicates = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	
	StaticMeshComp->SetSimulatePhysics(true);

	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetCollisionObjectType(ECC_Pawn);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	
	StaticMeshComp->BodyInstance.bUseCCD = true;

	StaticMeshComp->bGenerateOverlapEvents = true;
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->OnComponentHit.AddDynamic(this, &AGamePawn::OnHit);

	SetRootComponent(StaticMeshComp);

	PropComp = CreateDefaultSubobject<UPropComponent>("PropComp");

	MovementComp = nullptr;
	
	OwnerAttackComp = nullptr;
	OwnerSkillComp = nullptr;

	PrimaryActorTick.bCanEverTick = true;

	CansumeScale = 1.f;
}

void AGamePawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	HitAbleDisplayName = RoleName.ToString();

	OnAgilityAndQualityChanged();
}

void AGamePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	OnPossessedByControllerDelegate.Broadcast(NewController);
}


void AGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		ChangeStaminaTick(DeltaTime);
	}
}

void AGamePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentRoleSkin) { delete CurrentRoleSkin; }
	if (CurrentRoleForm) { delete CurrentRoleForm; }

	Super::EndPlay(EndPlayReason);
}

//////////////////////////////////////////////////////////////////////////
/// HitAble Interface
void AGamePawn::SetIsSelectedToHit(bool bInIsSelectedToHit)
{
	if (bInIsSelectedToHit)
	{
		StaticMeshComp->SetRenderCustomDepth(true);
		StaticMeshComp->CustomDepthStencilValue = 2.f;
	}
	else
	{
		StaticMeshComp->SetRenderCustomDepth(false);
		StaticMeshComp->CustomDepthStencilValue = 0.f;
	}
}

void AGamePawn::AcceptHitFrom(AActor* OtherActor, FVector NormalInpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		AEveryThingGameMode_Game* GameMode = Cast<AEveryThingGameMode_Game>(GetWorld()->GetAuthGameMode());
		if (GameMode && GameMode->CanTakeDamage(this, OtherActor))
		{
			IHitAbleInterface* HitableActor = Cast<IHitAbleInterface>(OtherActor);
			if (HitableActor)
			{
				AGamePawn* OtherPawn = Cast<AGamePawn>(OtherActor);
				if (OtherPawn)
				{
					float Damage = GameMode->GetDamageFromGamePawnHit(this, OtherPawn, NormalInpulse, Hit);
					if (Damage > 0.f)
					{
						FPointDamageEvent DamageEvent;
						DamageEvent.HitInfo = Hit;
						DamageEvent.Damage = Damage;
						TakeDamage(Damage, DamageEvent, OtherPawn->GetController(), OtherPawn);
					}
				}
				else
				{
					float Damage = GameMode->GetDamageFromHitableHit(this, NormalInpulse, Hit);
					if (Damage > 0.f)
					{
						FPointDamageEvent DamageEvent;
						DamageEvent.HitInfo = Hit;						
						DamageEvent.Damage = Damage;
						TakeDamage(Damage, DamageEvent, nullptr, OtherActor);
					}
				}
			}
			else
			{
				float Damage = GameMode->GetDamageFromActorHit(this, NormalInpulse, Hit);
				if (Damage > 0.f)
				{
					FPointDamageEvent DamageEvent;
					DamageEvent.HitInfo = Hit;
					DamageEvent.Damage = Damage;
					TakeDamage(Damage, DamageEvent, nullptr, OtherActor);
				}
			}
		}
	}
}

bool AGamePawn::CanBeSelectedToHit(AActor* Selector) const
{
	return !bIsDeath;
}

//////////////////////////////////////////////////////////////////////////
/// Hit
void AGamePawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	OnHitImplement(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
	OnHitDelegate.Broadcast(HitComp, OtherActor, OtherComp, NormalInpulse, Hit);
}

void AGamePawn::OnHitImplement(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	if (!bIsDeath) { AcceptHitFrom(OtherActor, NormalInpulse, Hit); }
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Form And Skin

void AGamePawn::ResetDefaultSkinAndFormFromDataTable()
{
	const FRoleInfo* RoleInfo;
	if (UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager()->GetRoleInfoFromName(RoleName, RoleInfo) && RoleInfo)
	{
		AllHaveRoleSkinName.AddUnique(RoleInfo->DefaultSkinName);
		OnAllHaveRoleSkinNameUpdate();
		ToggleToTargetSkin(RoleInfo->DefaultSkinName);

		AllHaveRoleFormName.AddUnique(RoleInfo->DefaultFormName);
		OnAllHaveRoleFormNameUpdate();
		ToggleToTargetForm(RoleInfo->DefaultFormName);
	}
}

//////////////////////////////////////////////////////////////////////////
/// Stamina
void AGamePawn::SpendStamina(float value)
{
	Stamina -= value;

	if (Stamina >= MaxStamina) { Stamina = MaxStamina; }
	if (Stamina <= 0.f) { Stamina = 0.f; }

	OnStaminaUpdateDelegate.Broadcast(Stamina);
}

void AGamePawn::ChangeStaminaTick(float DeltaTime)
{
	SpendStamina(-StaminaRecoverRate*DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
/// Cure, Damage, Death

void AGamePawn::Healed(AActor* Curer, float HealingValue)
{
	ChangeDurability(HealingValue);

	AEveryThingGameState_Game* OwnerETGS_G = Cast<AEveryThingGameState_Game>(GetWorld()->GetGameState());
	if (OwnerETGS_G) { OwnerETGS_G->OnGamePawnAcceptCure(this, Curer, HealingValue); }
}
void AGamePawn::ChangeDurability(float DurabilityOffset)
{	
	Durability += DurabilityOffset;
	if (Durability < 0.f) { Durability = 0.f; }
	if (Durability > MaxDurability) { Durability = MaxDurability; }

	OnDurabilityUpdateDelegate.Broadcast(Durability);
}

float AGamePawn::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ChangeDurability(-FinalDamage);

	LastDamageCauserActor = DamageCauser;
		
	AEveryThingGameState_Game* OwnerETGS_G = Cast<AEveryThingGameState_Game>(GetWorld()->GetGameState());
	if (OwnerETGS_G) { OwnerETGS_G->OnGamePawnAcceptDamage(this, DamageCauser, FinalDamage); }

	if (Durability == 0.f)
	{
		if (OwnerETGS_G) {OwnerETGS_G->OnGamePawnAcceptCriticalDamage(this, DamageCauser);}

		GamePawnDeath();
	}

	return FinalDamage;
}

void AGamePawn::GamePawnDeath()
{
	AEveryThingGameState_Game* OwnerETGS_G = Cast<AEveryThingGameState_Game>(GetWorld()->GetGameState());
	if (OwnerETGS_G) { OwnerETGS_G->OnGamePawnBeKilled(this, LastDamageCauserActor); };

	bIsDeath = true;

	StaticMeshComp->SetSimulatePhysics(false);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UMaterialInstanceConstant* DissolveMaterial = UEveryThingAssetManager::GetAssetManagerInstance()->GetMaterialFromName("Dissolve", true);
	UMaterialInstanceDynamic* DissolveDynamicMaterial = DissolveMaterial ? UMaterialInstanceDynamic::Create(DissolveMaterial, nullptr) : nullptr;
	DissolvePercent = 0.f;
	if (DissolveDynamicMaterial)
	{
		for (int32 i = 0; i < StaticMeshComp->GetNumMaterials(); ++i)
		{
			StaticMeshComp->SetMaterial(i, DissolveDynamicMaterial);
		}
	}


	GetWorldTimerManager().SetTimerForNextTick(this, &AGamePawn::DelayToDestroy);
}

void AGamePawn::DelayToDestroy()
{
	UWorld* Wolrd = GetWorld();
	if (Wolrd)
	{
		DissolvePercent += Wolrd->GetDeltaSeconds() * 0.3f;

		for (int32 i = 0; i < StaticMeshComp->GetNumMaterials(); ++i)
		{
			UMaterialInstanceDynamic* DissolveDynamicMaterial = Cast<UMaterialInstanceDynamic>(StaticMeshComp->GetMaterial(i));
			if (DissolveDynamicMaterial) { DissolveDynamicMaterial->SetScalarParameterValue("Percent", DissolvePercent); }
		}

		if (DissolvePercent < 1.f)
		{
			Wolrd->GetTimerManager().SetTimerForNextTick(this, &AGamePawn::DelayToDestroy);
		}
		else
		{
			Destroy();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Form
void AGamePawn::ToggleToNewFormWithIndex(int32 Index) { if (AllRoleFormName.IsValidIndex(Index)) { ToggleToTargetForm(AllRoleFormName[Index]); } }

bool AGamePawn::ServerToggleToTargetForm_Validate(const FName& FormName) { return true; }
void AGamePawn::ServerToggleToTargetForm_Implementation(const FName& FormName) { ToggleToTargetForm(FormName); }
void AGamePawn::OnRep_CurrentRoleFormName() { ToggleToTargetForm(CurrentRoleFormName); }

void AGamePawn::ToggleToTargetForm(const FName& FormName)
{
	if (!AllHaveRoleFormName.Contains(FormName))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("FormName"), FText::FromName(FormName));
		OnToggleToTargetSkinFailureDelegate.Broadcast(FormName, FText::Format(LOCTEXT("ToggleSkinWhenNotHaveTargetForm", "Don't have Target form name : {FormName}."), Arguments));
		return;
	}

	if (FormName == CurrentRoleFormName)
	{
		OnToggleToTargetFormFailureDelegate.Broadcast(FormName, LOCTEXT("ToggleSkinWhenSameForm", "Already is the target Form, needn't to toggle."));
		return;
	}
	
	if (Role == ROLE_AutonomousProxy && !HasAuthority())
	{
		ServerToggleToTargetForm(FormName);
	}

	if (CurrentRoleForm) { CurrentRoleForm->UnloadGamePawnForm(); delete CurrentRoleForm; }
	

	FRoleForm* TargetGamePawnForm = UGamePawnManager::CreateRoleForm(FormName, this);

	CurrentRoleFormName = FormName;
	CurrentRoleForm = TargetGamePawnForm;

	if (CurrentRoleForm) { CurrentRoleForm->LoadGamePawnForm(); }

	OnToggleToTargetSkinSuccessDelegate.Broadcast(FormName);
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn Skin

void AGamePawn::ToggleToNewSkinWithIndex(int32 Index) { if (AllRoleSkinName.IsValidIndex(Index)) { ToggleToTargetSkin(AllRoleSkinName[Index]); } }
bool AGamePawn::ServerToggleToTargetSkin_Validate(const FName& SkinName) { return true; }
void AGamePawn::ServerToggleToTargetSkin_Implementation(const FName& SkinName) { ToggleToTargetSkin(SkinName); }
void AGamePawn::OnRep_CurrentRoleSkinName() { ToggleToTargetSkin(CurrentRoleSkinName); }

void AGamePawn::ToggleToTargetSkin(const FName& SkinName)
{
	if (!AllHaveRoleSkinName.Contains(SkinName))
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("SkinName"), FText::FromName(SkinName));
		OnToggleToTargetSkinFailureDelegate.Broadcast(SkinName, FText::Format(LOCTEXT("ToggleSkinWhenNotHaveTargetSkin", "Don't have Target skin name : {SkinName}."), Arguments));
		return;
	}

	if (SkinName == CurrentRoleSkinName)
	{
		OnToggleToTargetSkinFailureDelegate.Broadcast(SkinName, LOCTEXT("ToggleSkinWhenSameSkin", "Already is the target Skin, needn't to toggle."));
		return;
	}
	

	if (Role == ROLE_AutonomousProxy && !HasAuthority())
	{
		ServerToggleToTargetSkin(SkinName);
	}
	
	if (CurrentRoleSkin) { CurrentRoleSkin->UnloadGamePawnSkin(); delete CurrentRoleSkin; }

	FRoleSkin* TargetGamePawnSkin = UGamePawnManager::CreateRoleSkin(SkinName, StaticMeshComp);
	CurrentRoleSkinName = SkinName;
	CurrentRoleSkin = TargetGamePawnSkin;

	if (CurrentRoleSkin) { CurrentRoleSkin->LoadGamePawnSkin(); }

	OnToggleToTargetSkinSuccessDelegate.Broadcast(SkinName);
}

//////////////////////////////////////////////////////////////////////////
/// Attack and Skill
void AGamePawn::ToggleToNewAttackComponent(UAttackComponent* InAttackComponent)
{
	OwnerAttackComp = InAttackComponent;

	APlayerController_Game* OwnerPlayerPC = Cast<APlayerController_Game>(GetController());
	if (OwnerPlayerPC) { OwnerPlayerPC->ToggleToNewAttackComponent(OwnerAttackComp); }
}

void AGamePawn::ToggleToNewSkillComponent(USkillComponent* InSkillComponent)
{
	OwnerSkillComp = InSkillComponent;

	APlayerController_Game* OwnerPlayerPC = Cast<APlayerController_Game>(GetController());
	if (OwnerPlayerPC) { OwnerPlayerPC->ToggleToNewSkillComponent(OwnerSkillComp); }
}

void AGamePawn::OnRep_OwnerAttackComp() { ToggleToNewAttackComponent(OwnerAttackComp); }
void AGamePawn::OnRep_OwnerSkillComp() { ToggleToNewSkillComponent(OwnerSkillComp); }


AActor* AGamePawn::TryToGetAttackTarget(float InMaxAttackDistance)
{
	IGamePawnControllerInterface* OwnerPawnGPC = Cast<IGamePawnControllerInterface>(GetController());

	AActor* AttackTarget = OwnerPawnGPC ? OwnerPawnGPC->GetAttackTarget() : nullptr;

	if (AttackTarget)
	{
		if ((AttackTarget->GetActorLocation() - GetActorLocation()).Size() < InMaxAttackDistance)
		{
			FCollisionQueryParams TraceParams;
			TraceParams.AddIgnoredActor(this);
			FHitResult Hit;
		
			GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), AttackTarget->GetActorLocation(), ECC_Visibility, TraceParams);

			if (Hit.GetActor() == nullptr || Hit.GetActor() == AttackTarget)
			{
				return AttackTarget;
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("-_- can't attack target actor, because has a block object"));
			}
		}	
		else
		{
			UE_LOG(LogTemp, Log, TEXT("-_- exceed the max attack distance"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("-_- not found attack target"));
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// Game Pawn info

void AGamePawn::ResetInfoFromDataTable(const FName& GamePawnName)
{
	UDataTable* GamePawnInfoDataDable = UEveryThingAssetManager::GetAssetManagerInstance()->GetDataTableFromName("GamePawn");
	if (GamePawnInfoDataDable)
	{
		FGamePawnInfo* GamePawnInfo = GamePawnInfoDataDable->FindRow<FGamePawnInfo>(GamePawnName, FString::Printf(TEXT("find game pawn %s base info"), *GamePawnName.ToString()));
		if (GamePawnInfo) { SetInfo(GamePawnInfo); }
	}
}

void AGamePawn::SetInfo(const FGamePawnInfo* InInfo)
{
	BaseInfo = *InInfo;
	
	MaxDurability = InInfo->MaxDurability;
	Durability = MaxDurability;
	OnDurabilityUpdate();
	OnMaxDurabilityUpdate();

	MaxStamina = InInfo->MaxStamina;
	Stamina = MaxStamina;
	OnStaminaUpdate();
	OnMaxStaminaUpdate();
	
	StaminaRecoverRate = MaxStamina / 8.f;

	Agility = InInfo->Agility;
	Quality = InInfo->Quality;
	QualityScale = InInfo->QualityScale;

	ConsumeForceScale = InInfo->ConsumeForceScale * 0.0001f;
	ConsumeImpluseScale = InInfo->ConsumeImpluseScale * 0.0001f;;
	ConsumeTorqueScale = InInfo->ConsumeTorqueScale * 0.0000001f;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnAgilityAndQualityChanged();
	}
}

//////////////////////////////////////////////////////////////////////////
/// Quality And Damping
void AGamePawn::OnAgilityAndQualityChanged()
{
	ResetQuality();

	ResetDamping();

	if (MovementComp) { MovementComp->UpdateAgilityAndQuality(Agility, Quality, QualityScale); }
}

void AGamePawn::ResetQuality()
{
	FBodyInstance* BodyInstance = StaticMeshComp->GetBodyInstance();
	if (BodyInstance)
	{
		BodyInstance->MassScale = QualityScale;
		BodyInstance->SetMassOverride(Quality);
		BodyInstance->UpdateMassProperties();
	}
}
void AGamePawn::ResetDamping()
{
	FBodyInstance* BodyInstance = StaticMeshComp->GetBodyInstance();
	if(BodyInstance)
	{
		BodyInstance->LinearDamping  = BaseInfo.LinearDamping;
		BodyInstance->AngularDamping = BaseInfo.AngularDamping;
		BodyInstance->UpdateDampingProperties();
	}
}


//////////////////////////////////////////////////////////////////////////
/// On Use Force 
void AGamePawn::OnConsumeForce(const FVector& Force)
{
	SpendStamina(GetConsumeForceValue(Force));
}

void AGamePawn::OnConsumeTorqueInRadians(const FVector& Torque)
{
	SpendStamina(GetConsumeTorqueInRadiansValue(Torque));
}

void AGamePawn::OnConsumeImpulse(const FVector& Impulse)
{
	SpendStamina(GetConsumeImpulseValue(Impulse));
}

bool AGamePawn::CanConsumeForce(const FVector& Force)
{
	return !bIsDeath && Stamina > GetConsumeForceValue(Force);
}

bool AGamePawn::CanConsumeTorqueInRadians(const FVector& Torque)
{
	return !bIsDeath && Stamina > GetConsumeTorqueInRadiansValue(Torque);
}

bool AGamePawn::CanConsumeImpulse(const FVector& Impulse)
{
	return !bIsDeath && Stamina > GetConsumeImpulseValue(Impulse);
}

void AGamePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePawn, CurrentRoleSkinName);
	DOREPLIFETIME(AGamePawn, CurrentRoleFormName);

	DOREPLIFETIME(AGamePawn, OwnerAttackComp);
	DOREPLIFETIME(AGamePawn, OwnerSkillComp);

	DOREPLIFETIME(AGamePawn, HitAbleDisplayName);
	DOREPLIFETIME(AGamePawn, BaseInfo);

	DOREPLIFETIME(AGamePawn, Durability);
	DOREPLIFETIME(AGamePawn, MaxDurability);
	DOREPLIFETIME(AGamePawn, Stamina);
	DOREPLIFETIME(AGamePawn, MaxStamina);

	DOREPLIFETIME(AGamePawn, Agility);
	DOREPLIFETIME(AGamePawn, Quality);
	DOREPLIFETIME(AGamePawn, QualityScale);
}


#undef LOCTEXT_NAMESPACE