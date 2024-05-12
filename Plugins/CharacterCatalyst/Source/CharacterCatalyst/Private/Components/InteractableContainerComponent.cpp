// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/InteractableContainerComponent.h"

#include "Components/InteractionComponent.h"


UInteractableContainerComponent::UInteractableContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UInteractableContainerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UInteractableContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UInteractableContainerComponent::AddInteractable(AActor* Actor)
{
	if (Actor->GetComponentByClass(UInteractionComponent::StaticClass()))
	{
		InteractableActors.Add(Actor);
		return true;
	}
	return false;
}

bool UInteractableContainerComponent::RemoveInteractable(AActor* Actor)
{
	if (InteractableActors.Remove(Actor) > 0) return true;
	return false;
}

bool UInteractableContainerComponent::GetInteractable(AActor*& Interactable)
{
	if (InteractableActors.IsEmpty()) return false;
	Interactable = InteractableActors[0];
	return true;
}

