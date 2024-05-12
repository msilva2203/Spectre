// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableContainerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERCATALYST_API UInteractableContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableContainerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Interactable Container")
	bool AddInteractable(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Interactable Container")
	bool RemoveInteractable(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Interactable Container")
	bool GetInteractable(AActor*& Interactable);

private:
	UPROPERTY()
	TArray<AActor*> InteractableActors;
};
