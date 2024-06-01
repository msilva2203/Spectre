// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Statics/CatalystStatics.h"

#include "Interfaces/CatalystInterface.h"
#include "Components/AttributeMapComponent.h"
#include "Components/IntegerAttributeComponent.h"
#include "Components/ScalarAttributeComponent.h"
#include "Components/ByteAttributeComponent.h"
#include "Components/BoolAttributeComponent.h"
#include "Components/ActorAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Containers/UnrealString.h"
#include "Data/GameTypeData.h"
#include "Data/MapData.h"
#include "GameStateCatalyst.h"
#include "GameModeCatalyst.h"
#include "Components/ChatComponent.h"
#include "Components/SettingsContainerComponent.h"

bool UCatalystStatics::OpenLevel(const UObject* WorldContextObject, UGameTypeData* GameType, UMapData* Map, FString ExtraOptions)
{
	if (GameType && Map)
	{
		FName LevelName = Map->Data.LevelID;
		FString Options = "?listen?game=" + GameType->Data.Alias + "?" + ExtraOptions;
		UGameplayStatics::OpenLevel(WorldContextObject, LevelName, true, Options);
		return true;
	}
	return false;
}

bool UCatalystStatics::ServerTravel(const UObject* WorldContextObject, UGameTypeData* GameType, UMapData* Map)
{
	if (GameType && Map)
	{
		FString URL = Map->Data.LevelID.ToString() + "?listen?game=" + GameType->Data.Alias;
		WorldContextObject->GetWorld()->ServerTravel(URL);
		return true;
	}
	return false;
}

AGameStateCatalyst* UCatalystStatics::GetGameStateCatalyst(const UObject* WorldContextObject)
{
	auto GS = UGameplayStatics::GetGameState(WorldContextObject);
	return Cast<AGameStateCatalyst>(GS);
}

AGameModeCatalyst* UCatalystStatics::GetGameModeCatalyst(const UObject* WorldContextObject)
{
	auto GM = UGameplayStatics::GetGameMode(WorldContextObject);
	return Cast<AGameModeCatalyst>(GM);
}

bool UCatalystStatics::GetChat(APlayerController* PlayerController, const uint8 ChatID, UChatComponent*& OutChat)
{
	if (PlayerController)
	{
		auto Comps = PlayerController->GetComponents();

		for (auto& Comp : Comps)
		{
			OutChat = Cast<UChatComponent>(Comp);
			if (OutChat)
			{
				if (OutChat->ChatID == ChatID)
					return true;
			}
		}
	}
	OutChat = nullptr;
	return false;
}

uint8 UCatalystStatics::GetActorTeam(AActor* Actor)
{
	if (auto CI = Cast<ICatalystInterface>(Actor))
	{
		return CI->GetTeam();
	}
	return 255;
}

bool UCatalystStatics::AreActorsTeammates(AActor* A, AActor* B)
{
	return GetActorTeam(A) == GetActorTeam(B);
}

bool UCatalystStatics::GetActorIntegerAttribute(AActor* Actor, FName AttributeID, UIntegerAttributeComponent*& OutAttribute)
{
	TSet<UActorComponent*> Components;
	if (!Actor) return false;
	auto MapComp = Actor->GetComponentByClass(UAttributeMapComponent::StaticClass());
	if (!MapComp) return false;
	auto Map = Cast<UAttributeMapComponent>(MapComp);
	if (!Map) return false;

	// Checks if attribute is already added to attribute map
	if (Map)
	{
		if (auto Attribute = Map->FindIntegerAttribute(AttributeID))
		{
			OutAttribute = Attribute;
			return true;
		}
	}
	// Finds attribute with ID and adds it to the attribute map
	Components = Actor->GetComponents();
	for (auto Comp : Components)
	{
		if (auto Attribute = Cast<UIntegerAttributeComponent>(Comp))
		{
			if (Attribute->ID == AttributeID)
			{
				if (Map)
				{
					Map->AddIntegerAttribute(Attribute);
				}
				OutAttribute = Attribute;
				return true;
			}
		}
	}
	return false;
}

bool UCatalystStatics::GetActorScalarAttribute(AActor* Actor, FName AttributeID, UScalarAttributeComponent*& OutAttribute)
{
	TSet<UActorComponent*> Components;
	if (!Actor) return false;
	auto MapComp = Actor->GetComponentByClass(UAttributeMapComponent::StaticClass());
	if (!MapComp) return false;
	auto Map = Cast<UAttributeMapComponent>(MapComp);
	if (!Map) return false;

	// Checks if attribute is already added to attribute map
	if (Map)
	{
		if (auto Attribute = Map->FindScalarAttribute(AttributeID))
		{
			OutAttribute = Attribute;
			return true;
		}
	}
	// Finds attribute with ID and adds it to the attribute map
	Components = Actor->GetComponents();
	for (auto Comp : Components)
	{
		if (auto Attribute = Cast<UScalarAttributeComponent>(Comp))
		{
			if (Attribute->ID == AttributeID)
			{
				if (Map)
				{
					Map->AddScalarAttribute(Attribute);
				}
				OutAttribute = Attribute;
				return true;
			}
		}
	}
	return false;
}

bool UCatalystStatics::GetActorByteAttribute(AActor* Actor, FName AttributeID, UByteAttributeComponent*& OutAttribute)
{
	TSet<UActorComponent*> Components;
	if (!Actor) return false;
	auto MapComp = Actor->GetComponentByClass(UAttributeMapComponent::StaticClass());
	if (!MapComp) return false;
	auto Map = Cast<UAttributeMapComponent>(MapComp);
	if (!Map) return false;

	// Checks if attribute is already added to attribute map
	if (Map)
	{
		if (auto Attribute = Map->FindByteAttribute(AttributeID))
		{
			OutAttribute = Attribute;
			return true;
		}
	}
	// Finds attribute with ID and adds it to the attribute map
	Components = Actor->GetComponents();
	for (auto Comp : Components)
	{
		if (auto Attribute = Cast<UByteAttributeComponent>(Comp))
		{
			if (Attribute->ID == AttributeID)
			{
				if (Map)
				{
					Map->AddByteAttribute(Attribute);
				}
				OutAttribute = Attribute;
				return true;
			}
		}
	}
	return false;
}

bool UCatalystStatics::GetActorBoolAttribute(AActor* Actor, FName AttributeID, UBoolAttributeComponent*& OutAttribute)
{
	TSet<UActorComponent*> Components;
	if (!Actor) return false;
	auto MapComp = Actor->GetComponentByClass(UAttributeMapComponent::StaticClass());
	if (!MapComp) return false;
	auto Map = Cast<UAttributeMapComponent>(MapComp);
	if (!Map) return false;

	// Checks if attribute is already added to attribute map
	if (Map)
	{
		if (auto Attribute = Map->FindBoolAttribute(AttributeID))
		{
			OutAttribute = Attribute;
			return true;
		}
	}
	// Finds attribute with ID and adds it to the attribute map
	Components = Actor->GetComponents();
	for (auto Comp : Components)
	{
		if (auto Attribute = Cast<UBoolAttributeComponent>(Comp))
		{
			if (Attribute->ID == AttributeID)
			{
				if (Map)
				{
					Map->AddBoolAttribute(Attribute);
				}
				OutAttribute = Attribute;
				return true;
			}
		}
	}
	return false;
}

bool UCatalystStatics::GetActorAttribute(AActor* Actor, FName AttributeID, UActorAttributeComponent*& OutAttribute)
{
	TSet<UActorComponent*> Components;
	if (!Actor) return false;
	auto MapComp = Actor->GetComponentByClass(UAttributeMapComponent::StaticClass());
	if (!MapComp) return false;
	auto Map = Cast<UAttributeMapComponent>(MapComp);
	if (!Map) return false;

	// Checks if attribute is already added to attribute map
	if (Map)
	{
		if (auto Attribute = Map->FindActorAttribute(AttributeID))
		{
			OutAttribute = Attribute;
			return true;
		}
	}
	// Finds attribute with ID and adds it to the attribute map
	Components = Actor->GetComponents();
	for (auto Comp : Components)
	{
		if (auto Attribute = Cast<UActorAttributeComponent>(Comp))
		{
			if (Attribute->ID == AttributeID)
			{
				if (Map)
				{
					Map->AddActorAttribute(Attribute);
				}
				OutAttribute = Attribute;
				return true;
			}
		}
	}
	return false;
}

void UCatalystStatics::GetGameSettings(const UObject* WorldContextObject, FSettingsContainer& Settings)
{
	if (auto GS = GetGameStateCatalyst(WorldContextObject))
	{
		Settings = GS->GameSettingsContainer->Settings;
	}
}

void UCatalystStatics::SetGameSettingOption(const UObject* WorldContextObject, FName SettingName, int32 NewOption)
{
	if (auto GS = GetGameStateCatalyst(WorldContextObject))
	{
		GS->GameSettingsContainer->SetSettingOption(SettingName, NewOption, true);
	}
}

void UCatalystStatics::SetGameSettingOptionByValue(const UObject* WorldContextObject, FName SettingName, const FString& Value)
{
	if (auto GS = GetGameStateCatalyst(WorldContextObject))
	{
		FSettingsContainer* ContainerRef = &(GS->GameSettingsContainer->Settings);
		if (ContainerRef)
		{
			int32 NewOption = ContainerRef->Settings.FindRef(SettingName).GetValueOptionIndex(Value);
			GS->GameSettingsContainer->SetSettingOption(SettingName, NewOption, true);
		}
	}
}

void UCatalystStatics::SetGameSettingCustomValue(const UObject* WorldContextObject, FName SettingName, const FString& Value)
{
	if (auto GS = GetGameStateCatalyst(WorldContextObject))
	{
		GS->GameSettingsContainer->SetSettingCustomValue(SettingName, Value, true);
	}
}

bool UCatalystStatics::GetValue(FSetting Setting, FString& OutValue)
{
	if (Setting.CurrentOption < 0)
	{
		OutValue = Setting.CustomValue;
		return true;
	}
	if (Setting.CurrentOption < Setting.Options.Num() && !Setting.Options.IsEmpty())
	{
		FSettingOption Option = Setting.Options[Setting.CurrentOption];
		OutValue = Option.Value;
		return true;
	}
	OutValue = "";
	return false;
}

bool UCatalystStatics::GetBoolValue(FSetting Setting, bool& OutValue)
{
	FString Value;
	if (GetValue(Setting, Value))
	{
		OutValue = Value.ToBool();
		return true;
	}
	return false;
}

bool UCatalystStatics::GetIntegerValue(FSetting Setting, int32& OutValue)
{
	FString Value;
	if (GetValue(Setting, Value))
	{
		OutValue = UKismetStringLibrary::Conv_StringToInt(Value);
		return true;
	}
	return false;
}

bool UCatalystStatics::GetByteValue(FSetting Setting, uint8& OutValue)
{
	int32 IntValue = 0;
	if (GetIntegerValue(Setting, IntValue))
	{
		OutValue = (uint8)IntValue;
		return true;
	}
	return false;
}

bool UCatalystStatics::GetScalarValue(FSetting Setting, float& OutValue)
{
	FString Value;
	if (GetValue(Setting, Value))
	{
		LexFromString(OutValue, *Value);
		return true;
	}
	return false;
}

void UCatalystStatics::SetOption(UPARAM(ref)FSetting& Setting, int32 NewOption)
{
	Setting.CurrentOption = FMath::Clamp<int32>(NewOption, -1, Setting.Options.Num() - 1);
}

void UCatalystStatics::SetOptionByValue(UPARAM(ref)FSetting& Setting, const FString& Value)
{
	for (int i = 0; i < Setting.Options.Num(); i++)
	{
		if (Setting.Options[i].Value == Value)
		{
			Setting.CurrentOption = i;
			return;
		}
	}
}

void UCatalystStatics::SetCustomValue(UPARAM(ref)FSetting& Setting, const FString& Value)
{
	Setting.CustomValue = Value;
}

void UCatalystStatics::GetSettingValue(FSettingsContainer SettingsContainer, FName SettingName, FString DefaultValue, FString& OutValue)
{
	auto Setting = SettingsContainer.Settings.Find(SettingName);
	if (Setting)
	{
		if (GetValue(*Setting, OutValue))
		{
			return;
		}
	}
	OutValue = DefaultValue;
}

void UCatalystStatics::GetBoolSettingValue(FSettingsContainer SettingsContainer, FName SettingName, bool DefaultValue, bool& OutValue)
{
	auto Setting = SettingsContainer.Settings.Find(SettingName);
	if (Setting)
	{
		if (GetBoolValue(*Setting, OutValue))
		{
			return;
		}
	}
	OutValue = DefaultValue;
}

void UCatalystStatics::GetIntegerSettingValue(FSettingsContainer SettingsContainer, FName SettingName, int32 DefaultValue, int32& OutValue)
{
	auto Setting = SettingsContainer.Settings.Find(SettingName);
	if (Setting)
	{
		if (GetIntegerValue(*Setting, OutValue))
		{
			return;
		}
	}
	OutValue = DefaultValue;
}

void UCatalystStatics::GetByteSettingValue(FSettingsContainer SettingsContainer, FName SettingName, uint8 DefaultValue, uint8& OutValue)
{
	auto Setting = SettingsContainer.Settings.Find(SettingName);
	if (Setting)
	{
		if (GetByteValue(*Setting, OutValue))
		{
			return;
		}
	}
	OutValue = DefaultValue;
}

void UCatalystStatics::GetScalarSettingValue(FSettingsContainer SettingsContainer, FName SettingName, float DefaultValue, float& OutValue)
{
	auto Setting = SettingsContainer.Settings.Find(SettingName);
	if (Setting)
	{
		if (GetScalarValue(*Setting, OutValue))
		{
			return;
		}
	}
	OutValue = DefaultValue;
}

void UCatalystStatics::SetSettingOption(UPARAM(ref)FSettingsContainer& SettingsContainer, FName SettingName, int32 NewOption)
{
	SetOption(*SettingsContainer.Settings.Find(SettingName), NewOption);
}

void UCatalystStatics::SetSettingOptionByValue(UPARAM(ref)FSettingsContainer& SettingsContainer, FName SettingName, const FString& Value)
{
	SetOptionByValue(*SettingsContainer.Settings.Find(SettingName), Value);
}

void UCatalystStatics::SetSettingCustomValue(UPARAM(ref)FSettingsContainer& SettingsContainer, FName SettingName, const FString& Value)
{
	SetCustomValue(*SettingsContainer.Settings.Find(SettingName), Value);
}
