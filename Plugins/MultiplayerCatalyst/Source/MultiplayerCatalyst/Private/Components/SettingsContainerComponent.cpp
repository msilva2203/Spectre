// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/SettingsContainerComponent.h"

#include "Statics/CatalystStatics.h"

#include "Net/UnrealNetwork.h"

USettingsContainerComponent::USettingsContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	if (DefaultSettings)
	{
		SetSettings(DefaultSettings->Settings);
	}
}


void USettingsContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(USettingsContainerComponent, SettingsValueArray, COND_None);
	DOREPLIFETIME_CONDITION(USettingsContainerComponent, SettingsKeyArray, COND_None);
}

void USettingsContainerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USettingsContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USettingsContainerComponent::SetSettings(FSettingsContainer NewSettings)
{
	Settings = NewSettings;

	SettingsValueArray.Empty();
	Settings.Settings.GenerateValueArray(SettingsValueArray);
	GetOwner()->ForceNetUpdate();

	OnGameSettingsUpdated.Broadcast();
}

void USettingsContainerComponent::SetSettingOption(FName SettingName, int32 NewOption, bool bReplicate)
{
	UCatalystStatics::SetSettingOption(Settings, SettingName, NewOption);
	if (bReplicate)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_SetSettingOption(SettingName, NewOption);
		}
	}
	OnGameSettingsUpdated.Broadcast();
}

void USettingsContainerComponent::SetSettingCustomValue(FName SettingName, const FString& NewValue, bool bReplicate)
{
	UCatalystStatics::SetSettingCustomValue(Settings, SettingName, NewValue);
	if (bReplicate)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_SetSettingCustomValue(SettingName, NewValue);
		}
	}
	OnGameSettingsUpdated.Broadcast();
}

void USettingsContainerComponent::NetAll_SetSettingOption_Implementation(FName SettingName, int32 NewOption)
{
	if (!GetOwner()->HasAuthority())
	{
		SetSettingOption(SettingName, NewOption, false);
	}
}

void USettingsContainerComponent::NetAll_SetSettingCustomValue_Implementation(FName SettingName, const FString& NewValue)
{
	if (!GetOwner()->HasAuthority())
	{
		SetSettingCustomValue(SettingName, NewValue, false);
	}
}

void USettingsContainerComponent::OnRep_SettingsValueArray()
{
	GenerateSettingsContainer();
}

void USettingsContainerComponent::OnRep_SettingsKeyArray()
{
	GenerateSettingsContainer();
}

void USettingsContainerComponent::GenerateSettingsContainer()
{
	int32 Size;
	if ((Size = SettingsKeyArray.Num()) == SettingsValueArray.Num())
	{
		Settings.Settings.Empty();
		for (int i = 0; i < Size; i++)
		{
			Settings.Settings.Add(SettingsKeyArray[i], SettingsValueArray[i]);
		}
		OnGameSettingsUpdated.Broadcast();
	}
}

