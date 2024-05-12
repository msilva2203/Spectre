// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/ActorAttributeComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UActorAttributeComponent::UActorAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
}

void UActorAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UActorAttributeComponent, Actor, AttributeReplicationCondition);
	DOREPLIFETIME(UActorAttributeComponent, bLocked);
}


// Called when the game starts
void UActorAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	bLocked = bStartLocked;
	OnRep_bLocked();
}


// Called every frame
void UActorAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActorAttributeComponent::SetActor(AActor* NewActor, bool bForceReplication)
{
	if (!IsLocked())
	{
		if (Actor != NewActor)
		{
			Actor = NewActor;
			GetOwner()->ForceNetUpdate();
			OnRep_Actor();

			if (bForceReplication)
			{
				NetAll_SetActor(Actor);
			}
		}
	}
}

void UActorAttributeComponent::SetLock(bool bNewValue)
{
	bLocked = bNewValue;

	GetOwner()->ForceNetUpdate();
	OnRep_bLocked();
}

bool UActorAttributeComponent::IsLocked() const
{
	return bLocked;
}

void UActorAttributeComponent::NetAll_SetActor_Implementation(AActor* NewActor)
{
	if (!GetOwner()->HasAuthority())
		SetActor(NewActor);
}

void UActorAttributeComponent::OnRep_Actor()
{
	OnActorAttributeChanged.Broadcast(Actor, Previous);
	Previous = Actor;
}

void UActorAttributeComponent::OnRep_bLocked()
{
	if (IsLocked())
	{
		OnActorAttributeLocked.Broadcast();
	}
	else
	{
		OnActorAttributeUnlocked.Broadcast();
	}
}

