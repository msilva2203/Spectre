// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/SettingsData.h"
#include "ServerCatalystSubsystem.generated.h"

class UGameTypeData;
class UMapData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTypeUpdated, UGameTypeData*, NewGameType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapUpdated, UMapData*, NewMap);

/**
 * Subsystem designed to handle server transitions with persistent data
 */
UCLASS()
class MULTIPLAYERCATALYST_API UServerCatalystSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Server Subsystem", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	void Open(const UObject* WorldContextObject, FString ExtraOptions);

	UFUNCTION(BlueprintCallable, Category = "Server Subsystem", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	void Travel(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Server Subsystem")
	void SetGameType(UGameTypeData* NewGameType);

	UFUNCTION(BlueprintCallable, Category = "Server Subsystem")
	void SetMap(UMapData* NewMap);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Server Subsystem")
	UGameTypeData* CurrentGameType;

	UPROPERTY(BlueprintReadOnly, Category = "Server Subsystem")
	UMapData* CurrentMap;

	UPROPERTY(BlueprintReadOnly, Category = "Server Subsystem")
	FSettingsContainer GameSettings;

	UPROPERTY(BlueprintAssignable, Category = "Server Subsystem")
	FOnGameTypeUpdated OnGameTypeUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Server Subsystem")
	FOnMapUpdated OnMapUpdated;
};
