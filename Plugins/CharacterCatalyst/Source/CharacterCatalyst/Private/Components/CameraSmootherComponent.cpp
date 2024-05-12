// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/CameraSmootherComponent.h"

UCameraSmootherComponent::UCameraSmootherComponent() :
	DefaultInterpSpeed(1.0f)
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCameraSmootherComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Gets the camera arm
	CameraArm = Cast<USpringArmComponent>(GetOwner()->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (CameraArm)
	{
		bActive = true;

		// Enable camera arm lag (lag amount will be controlled by the settings)
		CameraArm->bEnableCameraLag = true;
		CameraArm->bEnableCameraRotationLag = true;
	}
	else bActive = false;

	// Gets the camera
	Camera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
	if (Camera)
	{
		bActive = true;
	}
	else bActive = false;

	// Update with default settings
	if (bActive)
	{
		UpdateParams.ShoulderOffset.InterpSpeed = DefaultInterpSpeed;
		UpdateParams.Distance.InterpSpeed = DefaultInterpSpeed;
		UpdateParams.TranslationLagSpeed.InterpSpeed = DefaultInterpSpeed;
		UpdateParams.RotationLagSpeed.InterpSpeed = DefaultInterpSpeed;
		UpdateParams.FOV.InterpSpeed = DefaultInterpSpeed;

		SetToDefaultValues();
	}
		
}

void UCameraSmootherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bActive) return;

	//******************************************************************
	// Interpolate setting values
	//******************************************************************
	CurrentSettings.ShoulderOffset = InterpolateSetting(CurrentSettings.ShoulderOffset, DeltaTime, UpdateParams.ShoulderOffset);
	CurrentSettings.Distance = InterpolateSetting(CurrentSettings.Distance, DeltaTime, UpdateParams.Distance);
	CurrentSettings.TranslationLagSpeed = InterpolateSetting(CurrentSettings.TranslationLagSpeed, DeltaTime, UpdateParams.TranslationLagSpeed);
	CurrentSettings.RotationLagSpeed = InterpolateSetting(CurrentSettings.RotationLagSpeed, DeltaTime, UpdateParams.RotationLagSpeed);
	CurrentSettings.FOV = InterpolateSetting(CurrentSettings.FOV, DeltaTime, UpdateParams.FOV);

	//******************************************************************
	// Update camera
	//******************************************************************
	UpdateCamera(CurrentSettings, false);
}

void UCameraSmootherComponent::UpdateCamera(const FCameraSettings& Settings, bool bForceUpdate)
{
	if (!bActive) return;

	CurrentSettings = Settings;

	if (bForceUpdate)
	{
		FCameraSettingUpdateParam ForceParam;
		ForceParam.InterpSpeed = -1.0f;

		ForceParam.Value = CalculateShoulderOffset(Settings.ShoulderOffset);
		UpdateSetting(ForceParam, ECameraSetting::ShoulderOffset);

		ForceParam.Value = Settings.Distance;
		UpdateSetting(ForceParam, ECameraSetting::Distance);

		ForceParam.Value = Settings.TranslationLagSpeed;
		UpdateSetting(ForceParam, ECameraSetting::TranslationLagSpeed);

		ForceParam.Value = Settings.RotationLagSpeed;
		UpdateSetting(ForceParam, ECameraSetting::RotationLagSpeed);

		ForceParam.Value = Settings.FOV;
		UpdateSetting(ForceParam, ECameraSetting::FOV);
	}

	//******************************************************************
	// Shoulder Offset
	//******************************************************************
	CameraArm->SocketOffset.Y = Settings.ShoulderOffset;

	//******************************************************************
	// Distance
	//******************************************************************
	CameraArm->TargetArmLength = Settings.Distance;

	//******************************************************************
	// Translation Lag Speed
	//******************************************************************
	CameraArm->CameraLagSpeed = Settings.TranslationLagSpeed;

	//******************************************************************
	// Rotation Lag Speed
	//******************************************************************
	CameraArm->CameraRotationLagSpeed = Settings.RotationLagSpeed;

	//******************************************************************
	// Field Of View
	//******************************************************************
	Camera->SetFieldOfView(Settings.FOV);
}

void UCameraSmootherComponent::UpdateSetting(FCameraSettingUpdateParam UpdateParam, ECameraSetting Setting, uint8 CustomSetting)
{
	switch (Setting)
	{
	case ECameraSetting::ShoulderOffset:
		UpdateParam.Value = CalculateShoulderOffset(UpdateParam.Value);
		UpdateParams.ShoulderOffset = UpdateParam;
		PreviousSettings.ShoulderOffset = UpdateParam.Value;
		break;
	case ECameraSetting::Distance:
		UpdateParams.Distance = UpdateParam;
		PreviousSettings.Distance = UpdateParam.Value;
		break;
	case ECameraSetting::TranslationLagSpeed:
		UpdateParams.TranslationLagSpeed = UpdateParam;
		PreviousSettings.TranslationLagSpeed = UpdateParam.Value;
		break;
	case ECameraSetting::RotationLagSpeed:
		UpdateParams.RotationLagSpeed = UpdateParam;
		PreviousSettings.RotationLagSpeed = UpdateParam.Value;
		break;
	case ECameraSetting::FOV:
		UpdateParams.FOV = UpdateParam;
		PreviousSettings.FOV = UpdateParam.Value;
	case ECameraSetting::Custom:
		break;
	default:
		break;
	}
}

void UCameraSmootherComponent::SetCameraShoulder(const ECameraShoulder NewValue)
{
	CurrentShoulder = NewValue;
	UpdateSetting(UpdateParams.ShoulderOffset, ECameraSetting::ShoulderOffset);
}

void UCameraSmootherComponent::UpdateSettingToDefaultValue(float InterpSpeed, ECameraSetting Setting, uint8 CustomSetting)
{
	FCameraSettingUpdateParam Param;
	Param.InterpSpeed = InterpSpeed;

	switch (Setting)
	{
	case ECameraSetting::ShoulderOffset:
		Param.Value = DefaultSettings.ShoulderOffset;
		break;
	case ECameraSetting::Distance:
		Param.Value = DefaultSettings.Distance;
		break;
	case ECameraSetting::TranslationLagSpeed:
		Param.Value = DefaultSettings.TranslationLagSpeed;
		break;
	case ECameraSetting::RotationLagSpeed:
		Param.Value = DefaultSettings.RotationLagSpeed;
		break;
	case ECameraSetting::FOV:
		Param.Value = DefaultSettings.FOV;
		break;
	case ECameraSetting::Custom:
		return;
	default:
		return;
	}

	UpdateSetting(Param, Setting, CustomSetting);
}

void UCameraSmootherComponent::UpdateSettingToPreviousValue(float InterpSpeed, ECameraSetting Setting, uint8 CustomSetting)
{
	FCameraSettingUpdateParam Param;
	Param.InterpSpeed = InterpSpeed;

	switch (Setting)
	{
	case ECameraSetting::ShoulderOffset:
		Param.Value = PreviousSettings.ShoulderOffset;
		break;
	case ECameraSetting::Distance:
		Param.Value = PreviousSettings.Distance;
		break;
	case ECameraSetting::TranslationLagSpeed:
		Param.Value = PreviousSettings.TranslationLagSpeed;
		break;
	case ECameraSetting::RotationLagSpeed:
		Param.Value = PreviousSettings.RotationLagSpeed;
		break;
	case ECameraSetting::FOV:
		Param.Value = PreviousSettings.FOV;
		break;
	case ECameraSetting::Custom:
		return;
	default:
		return;
	}

	UpdateSetting(Param, Setting, CustomSetting);
}

void UCameraSmootherComponent::UpdateSettingInterpSpeed(float InterpSpeed, ECameraSetting Setting, uint8 CustomSetting)
{
	switch (Setting)
	{
	case ECameraSetting::ShoulderOffset:
		UpdateParams.ShoulderOffset.InterpSpeed = InterpSpeed;
		break;
	case ECameraSetting::Distance:
		UpdateParams.Distance.InterpSpeed = InterpSpeed;
		break;
	case ECameraSetting::TranslationLagSpeed:
		UpdateParams.TranslationLagSpeed.InterpSpeed = InterpSpeed;
		break;
	case ECameraSetting::RotationLagSpeed:
		UpdateParams.RotationLagSpeed.InterpSpeed = InterpSpeed;
		break;
	case ECameraSetting::FOV:
		UpdateParams.FOV.InterpSpeed = InterpSpeed;
		break;
	case ECameraSetting::Custom:
		break;
	}
}

void UCameraSmootherComponent::SetToDefaultValues()
{
	SetCameraShoulder(DefaultCameraShoulder);
	UpdateCamera(DefaultSettings, true);
}

float UCameraSmootherComponent::InterpolateSetting(float CurrentValue, float DeltaTime, const FCameraSettingUpdateParam& UpdateParam)
{
	return FMath::FInterpTo<float>(CurrentValue, UpdateParam.Value, DeltaTime, UpdateParam.InterpSpeed);
}

float UCameraSmootherComponent::CalculateShoulderOffset(float BaseValue)
{
	BaseValue = FMath::Abs<float>(BaseValue);

	switch (CurrentShoulder)
	{
	case ECameraShoulder::None:
		return 0.0f;
	case ECameraShoulder::Right:
		return BaseValue;
	case ECameraShoulder::Left:
		return BaseValue * -1.0f;
	}
	return 0.0f;
}

