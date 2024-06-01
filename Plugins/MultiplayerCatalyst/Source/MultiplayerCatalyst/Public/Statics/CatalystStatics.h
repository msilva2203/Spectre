// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/SettingsData.h"
#include "CatalystStatics.generated.h"

class UIntegerAttributeComponent;
class UScalarAttributeComponent;
class UByteAttributeComponent;
class UBoolAttributeComponent;
class UActorAttributeComponent;
class UGameTypeData;
class UMapData;
class AGameStateCatalyst;
class AGameModeCatalyst;
class UChatComponent;

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API UCatalystStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Transitions */

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static bool OpenLevel(const UObject* WorldContextObject, UGameTypeData* GameType, UMapData* Map, FString ExtraOptions);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject",UnsafeDuringActorConstruction = "true"))
	static bool ServerTravel(const UObject* WorldContextObject, UGameTypeData* GameType, UMapData* Map);

	/* Base Classes */

	UFUNCTION(BlueprintPure, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static AGameStateCatalyst* GetGameStateCatalyst(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static AGameModeCatalyst* GetGameModeCatalyst(const UObject* WorldContextObject);

	/* Chat */

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static bool GetChat(APlayerController* PlayerController, const uint8 ChatID, UChatComponent*& OutChat);

	/* Game Logic */

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static uint8 GetActorTeam(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static bool AreActorsTeammates(AActor* A, AActor* B);

	/* Actor Attributes */

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static bool GetActorIntegerAttribute(AActor* Actor, FName AttributeID, UIntegerAttributeComponent*& OutAttribute);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static bool GetActorScalarAttribute(AActor* Actor, FName AttributeID, UScalarAttributeComponent*& OutAttribute);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static bool GetActorByteAttribute(AActor* Actor, FName AttributeID, UByteAttributeComponent*& OutAttribute);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static bool GetActorBoolAttribute(AActor* Actor, FName AttributeID, UBoolAttributeComponent*& OutAttribute);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics")
	static bool GetActorAttribute(AActor* Actor, FName AttributeID, UActorAttributeComponent*& OutAttribute);

	/* Settings */

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void GetGameSettings(const UObject* WorldContextObject, FSettingsContainer& Settings);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void SetGameSettingOption(const UObject* WorldContextObject, FName SettingName, int32 NewOption);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void SetGameSettingOptionByValue(const UObject* WorldContextObject, FName SettingName, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Catalyst Statics", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void SetGameSettingCustomValue(const UObject* WorldContextObject, FName SettingName, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static bool GetValue(FSetting Setting, FString& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static bool GetBoolValue(FSetting Setting, bool& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static bool GetIntegerValue(FSetting Setting, int32& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static bool GetByteValue(FSetting Setting, uint8& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static bool GetScalarValue(FSetting Setting, float& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static void SetOption(UPARAM(ref) FSetting& Setting, int32 NewOption);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static void SetOptionByValue(UPARAM(ref) FSetting& Setting, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	static void SetCustomValue(UPARAM(ref) FSetting& Setting, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void GetSettingValue(FSettingsContainer SettingsContainer, FName SettingName, FString DefaultValue, FString& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void GetBoolSettingValue(FSettingsContainer SettingsContainer, FName SettingName, bool DefaultValue, bool& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void GetIntegerSettingValue(FSettingsContainer SettingsContainer, FName SettingName, int32 DefaultValue, int32& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void GetByteSettingValue(FSettingsContainer SettingsContainer, FName SettingName, uint8 DefaultValue, uint8& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void GetScalarSettingValue(FSettingsContainer SettingsContainer, FName SettingName, float DefaultValue, float& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void SetSettingOption(UPARAM(ref) FSettingsContainer& SettingsContainer, FName SettingName, int32 NewOption);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void SetSettingOptionByValue(UPARAM(ref) FSettingsContainer& SettingsContainer, FName SettingName, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Setting Container")
	static void SetSettingCustomValue(UPARAM(ref) FSettingsContainer& SettingsContainer, FName SettingName, const FString& Value);
};
