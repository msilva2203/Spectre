// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "CatalystActor.h"

#include "Components/AttributeMapComponent.h"

// Sets default values
ACatalystActor::ACatalystActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeMap = CreateDefaultSubobject<UAttributeMapComponent>(TEXT("Attribute Map"));
}

// Called when the game starts or when spawned
void ACatalystActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatalystActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

