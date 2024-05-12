// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Subsystems/ServerCatalystSubsystem.h"

#include "Data/GameTypeData.h"
#include "Statics/CatalystStatics.h"

void UServerCatalystSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UServerCatalystSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UServerCatalystSubsystem::Open(const UObject* WorldContextObject, FString ExtraOptions)
{
	UCatalystStatics::OpenLevel(WorldContextObject, CurrentGameType, CurrentMap, ExtraOptions);
}

void UServerCatalystSubsystem::Travel(const UObject* WorldContextObject)
{
	UCatalystStatics::ServerTravel(WorldContextObject, CurrentGameType, CurrentMap);
}

void UServerCatalystSubsystem::SetGameType(UGameTypeData* NewGameType)
{
	if (NewGameType)
	{
		CurrentGameType = NewGameType;
		if (CurrentGameType->Data.Settings)
		{
			GameSettings = CurrentGameType->Data.Settings->Settings;
		}
		OnGameTypeUpdated.Broadcast(CurrentGameType);
	}
}

void UServerCatalystSubsystem::SetMap(UMapData* NewMap)
{
	if (NewMap)
	{
		CurrentMap = NewMap;
		OnMapUpdated.Broadcast(CurrentMap);
	}
}
