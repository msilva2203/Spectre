// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/AttributeMapComponent.h"

#include "Components/IntegerAttributeComponent.h"
#include "Components/ScalarAttributeComponent.h"
#include "Components/ByteAttributeComponent.h"
#include "Components/ActorAttributeComponent.h"
#include "Components/BoolAttributeComponent.h"

// Sets default values for this component's properties
UAttributeMapComponent::UAttributeMapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttributeMapComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UIntegerAttributeComponent* UAttributeMapComponent::FindIntegerAttribute(FName ID)
{
	UActorComponent* Comp = AttributeMap.FindRef(ID);
	return Cast<UIntegerAttributeComponent>(Comp);
}

UScalarAttributeComponent* UAttributeMapComponent::FindScalarAttribute(FName ID)
{
	UActorComponent* Comp = AttributeMap.FindRef(ID);
	return Cast<UScalarAttributeComponent>(Comp);
}

UByteAttributeComponent* UAttributeMapComponent::FindByteAttribute(FName ID)
{
	UActorComponent* Comp = AttributeMap.FindRef(ID);
	return Cast<UByteAttributeComponent>(Comp);
}

UBoolAttributeComponent* UAttributeMapComponent::FindBoolAttribute(FName ID)
{
	UActorComponent* Comp = AttributeMap.FindRef(ID);
	return Cast<UBoolAttributeComponent>(Comp);
}

UActorAttributeComponent* UAttributeMapComponent::FindActorAttribute(FName ID)
{
	UActorComponent* Comp = AttributeMap.FindRef(ID);
	return Cast<UActorAttributeComponent>(Comp);
}

bool UAttributeMapComponent::AddIntegerAttribute(UIntegerAttributeComponent* NewComponent)
{
	if (!AttributeMap.Contains(NewComponent->ID))
	{
		AttributeMap.Add(NewComponent->ID, NewComponent);
		return true;
	}
	return false;
}

bool UAttributeMapComponent::AddScalarAttribute(UScalarAttributeComponent* NewComponent)
{
	if (!AttributeMap.Contains(NewComponent->ID))
	{
		AttributeMap.Add(NewComponent->ID, NewComponent);
		return true;
	}
	return false;
}

bool UAttributeMapComponent::AddByteAttribute(UByteAttributeComponent* NewComponent)
{
	if (!AttributeMap.Contains(NewComponent->ID))
	{
		AttributeMap.Add(NewComponent->ID, NewComponent);
		return true;
	}
	return false;
}

bool UAttributeMapComponent::AddBoolAttribute(UBoolAttributeComponent* NewComponent)
{
	if (!AttributeMap.Contains(NewComponent->ID))
	{
		AttributeMap.Add(NewComponent->ID, NewComponent);
		return true;
	}
	return false;
}

bool UAttributeMapComponent::AddActorAttribute(UActorAttributeComponent* NewComponent)
{
	if (!AttributeMap.Contains(NewComponent->ID))
	{
		AttributeMap.Add(NewComponent->ID, NewComponent);
		return true;
	}
	return false;
}

