// © 2024 Marco Silva. All Rights Reserved.


#include "Base/BaseEquipment.h"

ABaseEquipment::ABaseEquipment()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

