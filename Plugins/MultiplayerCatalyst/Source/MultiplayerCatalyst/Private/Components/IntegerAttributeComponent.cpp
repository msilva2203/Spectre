// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/IntegerAttributeComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UIntegerAttributeComponent::UIntegerAttributeComponent() :
	DefaultValue(0),
	MinValue(0),
	MaxValue(100),
	AttributeReplicationCondition(COND_None),
	bStartLocked(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
}

void UIntegerAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UIntegerAttributeComponent, AttributeValue, AttributeReplicationCondition);
	DOREPLIFETIME(UIntegerAttributeComponent, bLocked)
}


// Called when the game starts
void UIntegerAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	AttributeValue = DefaultValue;
	PreviousValue = DefaultValue;
	bLocked = bStartLocked;

	if (!IsLocked())
		OnRep_AttributeValue();
	OnRep_bLocked();
}


// Called every frame
void UIntegerAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIntegerAttributeComponent::SetAttributeValue(int32 NewValue, bool bForceReplication)
{
	if (!IsLocked())
	{
		NewValue = FMath::Clamp<int32>(NewValue, MinValue, MaxValue);

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

void UIntegerAttributeComponent::OffsetAttributeValue(int32 Offset)
{
	SetAttributeValue(AttributeValue + Offset);
}

void UIntegerAttributeComponent::ResetAttribute()
{
	SetAttributeValue(DefaultValue);
}

void UIntegerAttributeComponent::SetLock(bool bNewValue)
{
	bLocked = bNewValue;

	GetOwner()->ForceNetUpdate();
	OnRep_bLocked();
}

void UIntegerAttributeComponent::ForceNewValue(int32 NewValue)
{
	Server_SetAttributeValue(NewValue);
}

int32 UIntegerAttributeComponent::GetAttributeValue() const
{
	return AttributeValue;
}

bool UIntegerAttributeComponent::IsLocked() const
{
	return bLocked;
}

void UIntegerAttributeComponent::Server_SetAttributeValue_Implementation(int32 NewValue)
{
	SetAttributeValue(NewValue);
}

bool UIntegerAttributeComponent::Server_SetAttributeValue_Validate(int32 NewValue)
{
	return true;
}

void UIntegerAttributeComponent::NetAll_SetAttributeValue_Implementation(int32 NewValue)
{
	if (!GetOwner()->HasAuthority())
		SetAttributeValue(NewValue);
}

void UIntegerAttributeComponent::OnRep_AttributeValue()
{
	OnIntegerAttributeValueChanged.Broadcast(AttributeValue, PreviousValue);
	PreviousValue = AttributeValue;
}

void UIntegerAttributeComponent::OnRep_bLocked()
{
	if (IsLocked())
	{
		OnIntegerAttributeLocked.Broadcast();
	}
	else
	{
		OnIntegerAttributeUnlocked.Broadcast();
	}
}

