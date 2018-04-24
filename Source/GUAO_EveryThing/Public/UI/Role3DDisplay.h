// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Role3DDisplay.generated.h"



UCLASS()
class GUAO_EVERYTHING_API ARole3DDisplay : public AActor
{
	GENERATED_BODY()
	
public:
	ARole3DDisplay();
	
	virtual void BeginPlay() override;

	void AdjustUp(float AxisValue);
	void AdjustRight(float AxisValue);


	void ChangeSkin(const FName& SkinName);
	void ChangeRole(const FName& RoleName);

	FName CurrentRoleName;
	FName CurrentSkinName;
	class FRoleSkin* CurrentSkin;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCameraComponent* CameraComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneCaptureComponent2D* SceneCaptureComp2D;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;
};
