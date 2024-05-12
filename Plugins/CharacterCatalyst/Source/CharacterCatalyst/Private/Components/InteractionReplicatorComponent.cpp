// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/InteractionReplicatorComponent.h"

#include "Components/InteractionComponent.h"


UInteractionReplicatorComponent::UInteractionReplicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UInteractionReplicatorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UInteractionReplicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteractionReplicatorComponent::ReplicateInteraction(UInteractionComponent* InteractionComponent, AController* Controller, uint8 Action)
{
	if (!GetOwner()->HasAuthority() && InteractionComponent)
	{
		Server_Interact(InteractionComponent, Controller, Action);
	}
}

void UInteractionReplicatorComponent::Server_Interact_Implementation(UInteractionComponent* InteractionComponent, AController* Controller, uint8 Action)
{
	if (InteractionComponent)
	{
		InteractionComponent->Interact(Controller, Action);
	}
}

bool UInteractionReplicatorComponent::Server_Interact_Validate(UInteractionComponent* InteractionComponent, AController* Controller, uint8 Action)
{
	return true;
}

