// © 2024 Marco Silva. All Rights Reserved.


#include "Base/BaseEquipment.h"

ABaseEquipment::ABaseEquipment()
{
	PrimaryActorTick.bCanEverTick = true;


}

void ABaseEquipment::EquipmentAction(int32 Action, bool bValue)
{
	if (GetWorld()->GetNetMode() >= ENetMode::NM_Client)
	{

	}

	//NativeOnEquipmentAction(Action, bValue);
}

void ABaseEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

