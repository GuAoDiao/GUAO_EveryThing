// Fill out your copyright notice in the Description page of Project Settings.

#include "Role3DDisplay.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"


#include "Characters/Skin/GamePawnSkin.h"
#include "EveryThingAssetManager.h"
#include "Characters/GamePawnManager.h"

ARole3DDisplay::ARole3DDisplay()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComp->SetOrthoWidth(1024.f);
	SetRootComponent(CameraComp);

	SceneCaptureComp2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp2D"));
	SceneCaptureComp2D->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComp2D->OrthoWidth = 1024.f;
	
	SceneCaptureComp2D->SetupAttachment(CameraComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetCastShadow(false);
	StaticMeshComp->SetSimulatePhysics(true);
	FBodyInstance* BodyInstance = StaticMeshComp->GetBodyInstance();
	if (BodyInstance)
	{
		BodyInstance->bEnableGravity = false;
		BodyInstance->MassScale = 1.f;
		BodyInstance->SetMassOverride(100.f);
		BodyInstance->AngularDamping = 10.f;
	}
	StaticMeshComp->SetupAttachment(CameraComp);
	StaticMeshComp->SetRelativeLocation(FVector(500.f, 0.f, 0.f));

	CurrentRoleName = "Football";
	ChangeRole(CurrentRoleName);
}

void ARole3DDisplay::BeginPlay()
{
	Super::BeginPlay();

	UTextureRenderTarget2D* Texture = LoadObject<UTextureRenderTarget2D>(this, TEXT("TextureRenderTarget2D'/Game/EveryThing/UI/T_Character.T_Character'"));
	SceneCaptureComp2D->TextureTarget = Texture;
}

void ARole3DDisplay::ChangeRole(const FName& RoleName)
{
	UGamePawnManager* GamePawnManager = UEveryThingAssetManager::GetAssetManagerInstance()->GetGamePawnManager();
	if (GamePawnManager)
	{
		const FRoleDisplayInfo* RoleDisplayInfo;
		if (GamePawnManager->GetRoleDisplayInfo(RoleName, RoleDisplayInfo) && RoleDisplayInfo)
		{
			StaticMeshComp->SetStaticMesh(RoleDisplayInfo->DisplayStaticMesh);
			StaticMeshComp->SetRelativeRotation(RoleDisplayInfo->DisplayRotation);
			StaticMeshComp->SetRelativeScale3D(RoleDisplayInfo->DisplayScale);
			ChangeSkin(RoleDisplayInfo->DefaultSkinName);
		}
	}
}

void ARole3DDisplay::AdjustUp(float AxisValue)
{
	if (AxisValue != 0.f) { StaticMeshComp->AddTorqueInRadians(FVector(0.f, AxisValue * 100000000.f, 0.f)); }
}
void ARole3DDisplay::AdjustRight(float AxisValue)
{
	if (AxisValue != 0.f) { StaticMeshComp->AddTorqueInRadians(FVector(0.f, 0.f, AxisValue * -100000000.f)); }
}


void ARole3DDisplay::ChangeSkin(const FName& SkinName)
{
	if (SkinName == CurrentSkinName) { return; }

	FRoleSkin* TargetGamePawnSkin = UGamePawnManager::CreateRoleSkin(SkinName, StaticMeshComp);
	if (TargetGamePawnSkin)
	{
		if (CurrentSkin) {CurrentSkin->UnloadGamePawnSkin(); delete CurrentSkin;}	
		CurrentSkinName = SkinName;
		CurrentSkin = TargetGamePawnSkin;
		if (CurrentSkin) { CurrentSkin->LoadGamePawnSkin(); }
	}
}