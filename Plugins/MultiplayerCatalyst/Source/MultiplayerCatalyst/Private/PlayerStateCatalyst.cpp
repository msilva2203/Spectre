// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "PlayerStateCatalyst.h"

#include "Subsystems/SessionCatalystSubsystem.h"
#include "Components/AttributeMapComponent.h"
#include "Statics/CatalystStatics.h"
#include "Components/ByteAttributeComponent.h"
#include "Components/IntegerAttributeComponent.h"

#include "Net/UnrealNetwork.h"

APlayerStateCatalyst::APlayerStateCatalyst()
{
	PlayerTeamAttribute = CreateDefaultSubobject<UByteAttributeComponent>(TEXT("Player Team"));
	PlayerTeamAttribute->ID = TEXT("Player Team");
	PlayerTeamAttribute->DefaultValue = 0;

	AttributeMap = CreateDefaultSubobject<UAttributeMapComponent>(TEXT("Attribute Map"));
}

void APlayerStateCatalyst::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APlayerStateCatalyst::BeginPlay()
{
	Super::BeginPlay();

	SessionSubsystem = GetWorld()->GetSubsystem<USessionCatalystSubsystem>();
}

void APlayerStateCatalyst::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (SessionSubsystem)
	{
		SessionSubsystem->RemovePlayer(this);
	}
}

void APlayerStateCatalyst::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (auto NewPlayerState = Cast<APlayerStateCatalyst>(PlayerState))
	{
		//...
		NewPlayerState->PlayerTeamAttribute->SetAttributeValue(GetTeam());
	}
}

void APlayerStateCatalyst::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

uint8 APlayerStateCatalyst::GetTeam()
{
	return PlayerTeamAttribute->GetAttributeValue();
}

void APlayerStateCatalyst::OnRep_PlayerName()
{
	if (SessionSubsystem)
	{
		SessionSubsystem->AddPlayer(this);
	}
}

void APlayerStateCatalyst::ps_set_player_team(uint8 new_team)
{
	PlayerTeamAttribute->ForceNewValue(new_team);
}

void APlayerStateCatalyst::ps_set_int_attribute(FName name, int32 new_value)
{
	UIntegerAttributeComponent* Attribute = nullptr;
	if (UCatalystStatics::GetActorIntegerAttribute(this, name, Attribute))
	{
		Attribute->ForceNewValue(new_value);
	}
}
