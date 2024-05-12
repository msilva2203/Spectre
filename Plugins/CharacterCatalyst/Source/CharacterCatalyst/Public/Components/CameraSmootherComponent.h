// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CameraSmootherComponent.generated.h"

UENUM(BlueprintType)
enum class ECameraSetting : uint8
{
	ShoulderOffset          UMETA(DisplayName = "SHOULDER_OFFSET"),
	Distance                UMETA(DisplayName = "DISTANCE"),
	TranslationLagSpeed     UMETA(DisplayName = "TRANSLATION_LAG_SPEED"),
	RotationLagSpeed        UMETA(DisplayName = "ROTATION_LAG_SPEED"),
	FOV						UMETA(DisplayName = "FOV"),
	Custom                  UMETA(DisplayName = "CUSTOM")
};

UENUM(BlueprintType)
enum class ECameraShoulder : uint8
{
	None                    UMETA(DisplayName = "NONE"),
	Right                   UMETA(DisplayName = "RIGHT"),
	Left                    UMETA(DisplayName = "LEFT")
};

USTRUCT(BlueprintType)
struct FCameraSettingUpdateParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting Update Param")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting Update Param")
	float InterpSpeed;
};

USTRUCT(BlueprintType)
struct FCameraSettingUpdateParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting Update Params")
	FCameraSettingUpdateParam ShoulderOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting Update Params")
	FCameraSettingUpdateParam Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting Update Params")
	FCameraSettingUpdateParam TranslationLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting Update Params")
	FCameraSettingUpdateParam RotationLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting Update Params")
	FCameraSettingUpdateParam FOV;
};

USTRUCT(BlueprintType)
struct FCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting")
	float ShoulderOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting")
	float TranslationLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting")
	float RotationLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setting")
	float FOV;
};


UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent) )
class CHARACTERCATALYST_API UCameraSmootherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraSmootherComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Camera Smoother")
	void UpdateCamera(const FCameraSettings& Settings, bool bForceUpdate = false);

	UFUNCTION(BlueprintCallable, Category = "Camera Smoother")
	void UpdateSetting(FCameraSettingUpdateParam UpdateParam, ECameraSetting Setting, uint8 CustomSetting = 0);

	UFUNCTION(BlueprintCallable, Category = "Camera Smoother")
	void SetCameraShoulder(const ECameraShoulder NewValue);

	UFUNCTION(BlueprintCallable, Category = "Camera Smoother")
	void UpdateSettingToDefaultValue(float InterpSpeed, ECameraSetting Setting, uint8 CustomSetting = 0);

	UFUNCTION(BlueprintCallable, Category = "Camera Smoother")
	void UpdateSettingToPreviousValue(float InterpSpeed, ECameraSetting Setting, uint8 CustomSetting = 0);

	UFUNCTION(BlueprintCallable, Category = "Camera Smoother")
	void UpdateSettingInterpSpeed(float InterpSpeed, ECameraSetting Setting, uint8 CustomSetting = 0);

	UFUNCTION(BlueprintCallable, Category = "Camera Smoother")
	void SetToDefaultValues();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Smoother Config")
	FCameraSettings DefaultSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Smoother Config")
	ECameraShoulder DefaultCameraShoulder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Smoother Config")
	float DefaultInterpSpeed;

	UPROPERTY(BlueprintReadOnly,Category = "Camera Smoother")
	FCameraSettings CurrentSettings;

	UPROPERTY(BlueprintReadOnly, Category = "Camera Smoother")
	ECameraShoulder CurrentShoulder;

private:
	UPROPERTY()
	USpringArmComponent* CameraArm;

	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	FCameraSettingUpdateParams UpdateParams;

	UPROPERTY()
	FCameraSettings PreviousSettings;

	UPROPERTY()
	bool bActive;

private:
	UFUNCTION()
	float InterpolateSetting(float CurrentValue, float DeltaTime, const FCameraSettingUpdateParam& UpdateParam);

	UFUNCTION()
	float CalculateShoulderOffset(float BaseValue);
};
