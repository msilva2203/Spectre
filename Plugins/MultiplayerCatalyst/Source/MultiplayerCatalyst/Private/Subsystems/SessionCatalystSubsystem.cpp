// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Subsystems/SessionCatalystSubsystem.h"

#include "Components/AttributeMapComponent.h"
#include "Components/IntegerAttributeComponent.h"
#include "Components/ScalarAttributeComponent.h"

void USessionCatalystSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USessionCatalystSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USessionCatalystSubsystem::AddPlayer(APlayerStateCatalyst* NewPlayerState)
{
	if (!SessionPlayerStates.Contains(NewPlayerState))
	{
		SessionPlayerStates.Add(NewPlayerState);
		OnPlayerStateAdded.Broadcast(NewPlayerState);
	}
}

void USessionCatalystSubsystem::RemovePlayer(APlayerStateCatalyst* Removing)
{
	if (SessionPlayerStates.Contains(Removing))
	{
		SessionPlayerStates.Remove(Removing);
		OnPlayerStateRemoved.Broadcast(Removing);
	}
}
