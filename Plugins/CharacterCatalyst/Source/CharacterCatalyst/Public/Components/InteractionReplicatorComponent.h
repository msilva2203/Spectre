// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionReplicatorComponent.generated.h"

class UInteractionComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERCATALYST_API UInteractionReplicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionReplicatorComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void ReplicateInteraction(UInteractionComponent* InteractionComponent, AController* Controller, uint8 Action);

private:
	UFUNCTION(Server, Reliable)
	void Server_Interact(UInteractionComponent* InteractionComponent, AController* Controller, uint8 Action);
	void Server_Interact_Implementation(UInteractionComponent* InteractionComponent, AController* Controller, uint8 Action);
	bool Server_Interact_Validate(UInteractionComponent* InteractionComponent, AController* Controller, uint8 Action);
};
