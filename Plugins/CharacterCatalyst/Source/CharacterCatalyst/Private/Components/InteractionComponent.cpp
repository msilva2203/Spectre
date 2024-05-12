// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/InteractionComponent.h"

#include "Components/InteractionReplicatorComponent.h"
#include "Kismet/GameplayStatics.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteractionComponent::Interact(AController* Controller, uint8 Action)
{
	if (!GetOwner()->HasAuthority())
	{
		auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		auto ReplicatorComp = PlayerController->FindComponentByClass<UInteractionReplicatorComponent>();
		if (ReplicatorComp)
		{
			ReplicatorComp->ReplicateInteraction(this, Controller, Action);
		}
	}
	OnInteraction.Broadcast(Controller, Action);
}
