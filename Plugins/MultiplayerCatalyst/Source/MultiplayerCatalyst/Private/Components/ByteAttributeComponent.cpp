// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/ByteAttributeComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UByteAttributeComponent::UByteAttributeComponent() :
	DefaultValue(0),
	MinValue(0),
	MaxValue(255),
	AttributeReplicationCondition(COND_None),
	bStartLocked(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	AttributeValue = DefaultValue;
	PreviousValue = DefaultValue;
	bLocked = bStartLocked;

	// ...
}

void UByteAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UByteAttributeComponent, AttributeValue, AttributeReplicationCondition);
	DOREPLIFETIME(UByteAttributeComponent, bLocked)
}


// Called when the game starts
void UByteAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocked())
		OnRep_AttributeValue();
	OnRep_bLocked();
}


// Called every frame
void UByteAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UByteAttributeComponent::SetAttributeValue(uint8 NewValue, bool bForceReplication)
{
	if (!IsLocked())
	{
		NewValue = FMath::Clamp<uint8>(NewValue, MinValue, MaxValue);

		if (AttributeValue != NewValue)
		{
			AttributeValue = NewValue;
			GetOwner()->ForceNetUpdate();
			OnRep_AttributeValue();

			if (bForceReplication)
			{
				NetAll_SetAttributeValue(AttributeValue);
			}
		}
	}
}

void UByteAttributeComponent::OffsetAttributeValue(uint8 Offset)
{
	SetAttributeValue(AttributeValue + Offset);
}

void UByteAttributeComponent::ResetAttribute()
{
	SetAttributeValue(DefaultValue);
}

void UByteAttributeComponent::SetLock(bool bNewValue)
{
	bLocked = bNewValue;

	GetOwner()->ForceNetUpdate();
	OnRep_bLocked();
}

void UByteAttributeComponent::ForceNewValue(uint8 NewValue)
{
	Server_SetAttributeValue(NewValue);
}

uint8 UByteAttributeComponent::GetAttributeValue() const
{
	return AttributeValue;
}

bool UByteAttributeComponent::IsLocked() const
{
	return bLocked;
}

void UByteAttributeComponent::Server_SetAttributeValue_Implementation(uint8 NewValue)
{
	SetAttributeValue(NewValue);
}

bool UByteAttributeComponent::Server_SetAttributeValue_Validate(uint8 NewValue)
{
	return true;
}

void UByteAttributeComponent::NetAll_SetAttributeValue_Implementation(uint8 NewValue)
{
	if (!GetOwner()->HasAuthority())
		SetAttributeValue(NewValue);
}

void UByteAttributeComponent::OnRep_AttributeValue()
{
	OnByteAttributeValueChanged.Broadcast(AttributeValue, PreviousValue);
	PreviousValue = AttributeValue;
}

void UByteAttributeComponent::OnRep_bLocked()
{
	if (IsLocked())
	{
		OnByteAttributeLocked.Broadcast();
	}
	else
	{
		OnByteAttributeUnlocked.Broadcast();
	}
}

