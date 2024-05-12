// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "PlayerControllerCatalyst.h"

#include "Components/AttributeMapComponent.h"
#include "Statics/CatalystStatics.h"
#include "GameFramework/PlayerState.h"

APlayerControllerCatalyst::APlayerControllerCatalyst()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeMap = CreateDefaultSubobject<UAttributeMapComponent>(TEXT("Attribute Map"));
}

void APlayerControllerCatalyst::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetupPlayerController();
	}
}

void APlayerControllerCatalyst::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (PreviousPawn)
	{
		PreviousPawn->K2_DestroyActor();
	}
	PreviousPawn = InPawn;
}

void APlayerControllerCatalyst::OnUnPossess()
{
	Super::OnUnPossess();

	if (PreviousPawn)
	{
		PreviousPawn->K2_DestroyActor();
	}
}

void APlayerControllerCatalyst::SetupPlayerController()
{
	OnSetupPlayerController();
}

void APlayerControllerCatalyst::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetupPlayerController();
}

uint8 APlayerControllerCatalyst::GetTeam()
{
	return UCatalystStatics::GetActorTeam(PlayerState);
}

void APlayerControllerCatalyst::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
