// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "GameStateCatalyst.h"

#include "Components/AttributeMapComponent.h"
#include "Subsystems/ServerCatalystSubsystem.h"
#include "Components/SettingsContainerComponent.h"
#include "Components/BoolAttributeComponent.h"

#include "Net/UnrealNetwork.h"

AGameStateCatalyst::AGameStateCatalyst()
{
	GameSettingsContainer = CreateDefaultSubobject<USettingsContainerComponent>(TEXT("Game Settings Container"));

	InProgressAttribute = CreateDefaultSubobject<UBoolAttributeComponent>(TEXT("In Progress Attribute"));
	InProgressAttribute->ID = "In Progress";
	InProgressAttribute->DefaultValue = false;

	AttributeMap = CreateDefaultSubobject<UAttributeMapComponent>(TEXT("Attribute Map"));
}

void AGameStateCatalyst::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateCatalyst, CurrentGameType);
	DOREPLIFETIME(AGameStateCatalyst, CurrentMap);
}

void AGameStateCatalyst::BeginPlay()
{
	Super::BeginPlay();

	ServerSubsystem = GetGameInstance()->GetSubsystem<UServerCatalystSubsystem>();

	// Keeps track of modifications
	if (HasAuthority())
	{
		ServerSubsystem->OnGameTypeUpdated.AddDynamic(this, &AGameStateCatalyst::SetCurrentGameType);
		ServerSubsystem->OnMapUpdated.AddDynamic(this, &AGameStateCatalyst::SetCurrentMap);
	}
	GameSettingsContainer->OnGameSettingsUpdated.AddDynamic(this, &AGameStateCatalyst::OnGameSettingsUpdated);

	// Update current game type based on server subsystem
	if (ServerSubsystem->CurrentGameType)
	{
		SetCurrentGameType(ServerSubsystem->CurrentGameType);
	}
	else
	{
		// Update server subsystem game type with default game state type
		if (HasAuthority())
			ServerSubsystem->SetGameType(CurrentGameType);
		else
			OnRep_CurrentGameType();
	}
	// Update current map based on server subsystem
	if (ServerSubsystem->CurrentMap)
	{
		SetCurrentMap(ServerSubsystem->CurrentMap);
	}
	else
	{
		// Update server subsystem map with default game state map
		if (HasAuthority())
			ServerSubsystem->SetMap(CurrentMap);
		else
			OnRep_CurrentMap();
	}
	// Update settings container with server settings
	if (!ServerSubsystem->GameSettings.Settings.IsEmpty())
	{
		GameSettingsContainer->SetSettings(ServerSubsystem->GameSettings);
	}
}

void AGameStateCatalyst::SetCurrentGameType(UGameTypeData* NewGameType)
{
	CurrentGameType = NewGameType;
	ForceNetUpdate();
	OnRep_CurrentGameType();
}

void AGameStateCatalyst::SetCurrentMap(UMapData* NewMap)
{
	CurrentMap = NewMap;
	ForceNetUpdate();
	OnRep_CurrentMap();
}

void AGameStateCatalyst::OnGameSettingsUpdated()
{
	ServerSubsystem->GameSettings = GameSettingsContainer->Settings;
}

void AGameStateCatalyst::OnRep_CurrentGameType()
{
	if (!HasAuthority())
	{
		ServerSubsystem->SetGameType(CurrentGameType);
	}
}

void AGameStateCatalyst::OnRep_CurrentMap()
{
	if (!HasAuthority())
	{
		ServerSubsystem->SetMap(CurrentMap);
	}
}
