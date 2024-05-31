// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "CharacterCatalyst.h"

#include "Components/AttributeMapComponent.h"
#include "Statics/CatalystStatics.h"
#include "GameFramework/PlayerState.h"

ACharacterCatalyst::ACharacterCatalyst()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeMap = CreateDefaultSubobject<UAttributeMapComponent>(TEXT("Attribute Map"));
}

void ACharacterCatalyst::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetupCharacter();
	}
}

void ACharacterCatalyst::SetupCharacter()
{
	OnSetupCharacter();
}

void ACharacterCatalyst::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetupCharacter();
}

uint8 ACharacterCatalyst::GetTeam()
{
	return UCatalystStatics::GetActorTeam(GetPlayerState());
}

void ACharacterCatalyst::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterCatalyst::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

