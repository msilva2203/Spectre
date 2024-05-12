// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/BoolAttributeComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBoolAttributeComponent::UBoolAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	AttributeValue = DefaultValue;
	bLocked = bStartLocked;
}


void UBoolAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBoolAttributeComponent, AttributeValue, AttributeReplicationCondition);
	DOREPLIFETIME(UBoolAttributeComponent, bLocked);
}

// Called when the game starts
void UBoolAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocked())
		OnRep_AttributeValue();
	OnRep_bLocked();
}


// Called every frame
void UBoolAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBoolAttributeComponent::SetAttributeValue(bool NewValue, bool bForceReplication)
{
	if (!IsLocked())
	{
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

void UBoolAttributeComponent::ResetAttribute()
{
	SetAttributeValue(DefaultValue);
}

void UBoolAttributeComponent::SetLock(bool bNewValue)
{
	bLocked = bNewValue;

	GetOwner()->ForceNetUpdate();
	OnRep_bLocked();
}

void UBoolAttributeComponent::ForceNewValue(bool NewValue)
{

}

bool UBoolAttributeComponent::GetAttributeValue() const
{
	return AttributeValue;
}

bool UBoolAttributeComponent::IsLocked() const
{
	return bLocked;
}

void UBoolAttributeComponent::Server_SetAttributeValue_Implementation(bool NewValue)
{
}

bool UBoolAttributeComponent::Server_SetAttributeValue_Validate(bool NewValue)
{
	return false;
}

void UBoolAttributeComponent::NetAll_SetAttributeValue_Implementation(bool NewValue)
{
	if (!GetOwner()->HasAuthority())
		SetAttributeValue(NewValue);
}

void UBoolAttributeComponent::OnRep_AttributeValue()
{
	OnBoolAttributeValueChanged.Broadcast(AttributeValue);
}

void UBoolAttributeComponent::OnRep_bLocked()
{
	if (IsLocked())
	{
		OnBoolAttributeLocked.Broadcast();
	}
	else
	{
		OnBoolAttributeUnlocked.Broadcast();
	}
}
