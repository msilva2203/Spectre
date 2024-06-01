// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "GameModeCatalyst.h"

#include "GameStateCatalyst.h"
#include "PlayerControllerCatalyst.h"
#include "PlayerStateCatalyst.h"
#include "CharacterCatalyst.h"
#include "Subsystems/ServerCatalystSubsystem.h"
#include "Components/AttributeMapComponent.h"
#include "Statics/CatalystStatics.h"
#include "Components/ByteAttributeComponent.h"
#include "Data/GameTypeData.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/PlayerSpawner.h"
#include "Components/BoolAttributeComponent.h"

AGameModeCatalyst::AGameModeCatalyst()
{
	GameStateClass = AGameStateCatalyst::StaticClass();
	PlayerControllerClass = APlayerControllerCatalyst::StaticClass();
	PlayerStateClass = APlayerStateCatalyst::StaticClass();
	DefaultPawnClass = ACharacterCatalyst::StaticClass();

	AttributeMap = CreateDefaultSubobject<UAttributeMapComponent>(TEXT("Attribute Map"));
}

int32 AGameModeCatalyst::GetTeamSize(uint8 Team)
{
	int32 Size = 0;
	for (auto Player : SessionPlayers)
	{
		if ((UCatalystStatics::GetActorTeam(Player)) == Team)
		{
			Size++;
		}
	}
	return Size;
}

void AGameModeCatalyst::AssignTeamToPlayer(APlayerControllerCatalyst* Player)
{
	UByteAttributeComponent* PlayerTeamAttribute;
	if (!UCatalystStatics::GetActorByteAttribute(Player->PlayerState, "Player Team", PlayerTeamAttribute)) return;

	// Assign new team to player if player has no team assigned
	if ((PlayerTeamAttribute->GetAttributeValue()) == 0)
	{
		uint8 AvailableTeam = GetAvailableTeam();
		PlayerTeamAttribute->SetAttributeValue(AvailableTeam);
	}
}

uint8 AGameModeCatalyst::GetAvailableTeam()
{
	uint8 NewTeam = 1;
	int32 NumTeams = 2;
	int32 PreviousTeamSize = 999;

	if (ServerSubsystem && ServerSubsystem->CurrentGameType)
	{
		NumTeams = ServerSubsystem->CurrentGameType->Data.NumTeams;
	}
	for (int32 Team = 1; Team < (NumTeams + 1); Team++)
	{
		int32 TeamSize = GetTeamSize(Team);
		if (TeamSize == 0)
		{
			NewTeam = Team;
			return NewTeam;
		}
		if (TeamSize < PreviousTeamSize)
		{
			NewTeam = Team;
			PreviousTeamSize = TeamSize;
		}
	}
	return NewTeam;
}

void AGameModeCatalyst::Start()
{
	// Set In Progress Attribute to true
	UBoolAttributeComponent* InProgressAttribute;
	if (UCatalystStatics::GetActorBoolAttribute(GameState, "In Progress", InProgressAttribute))
	{
		InProgressAttribute->SetAttributeValue(true);
	}

	OnStart();
	StartPlayers();
}

void AGameModeCatalyst::Pause()
{
	// Set In Progress Attribute to false
	UBoolAttributeComponent* InProgressAttribute;
	if (UCatalystStatics::GetActorBoolAttribute(GameState, "In Progress", InProgressAttribute))
	{
		InProgressAttribute->SetAttributeValue(false);
	}

	OnPause();

	for (auto Player : SessionPlayers)
	{
		Player->UnPossess();
	}
}

void AGameModeCatalyst::Resume()
{
	// Set In Progress Attribute to true
	UBoolAttributeComponent* InProgressAttribute;
	if (UCatalystStatics::GetActorBoolAttribute(GameState, "In Progress", InProgressAttribute))
	{
		InProgressAttribute->SetAttributeValue(true);
	}

	OnResume();
	StartPlayers();
}

void AGameModeCatalyst::StartPlayers()
{
	OnStartPlayers();

	for (auto Player : SessionPlayers)
	{
		SpawnPlayer(Player, true);
	}
}

void AGameModeCatalyst::SpawnPlayer(APlayerControllerCatalyst* Player, bool bStarterSpawner)
{
	if (Player)
	{
		uint8 PlayerTeam = UCatalystStatics::GetActorTeam(Player);
		auto Spawner = FindAvailableSpawner(PlayerTeam, bStarterSpawner);

		if (Spawner)
			Spawner->SpawnPlayer(Player);
	}
}

APlayerSpawner* AGameModeCatalyst::FindAvailableSpawner(uint8 Team, bool bStarterSpawner)
{
	for (auto Spawner : PlayerSpawners)
	{
		if (Spawner->SpawnerTeam == Team || Spawner->SpawnerTeam == 0)
		{
			if (bStarterSpawner)
			{
				if (!Spawner->bStarterSpawner) continue;
			}
			return Spawner;
		}
	}
	return nullptr;
}

void AGameModeCatalyst::SendChatMessageToPlayer(APlayerController* Player, const FChatMessage& ChatMessage, uint8 ChatID)
{
	UChatComponent* Chat;
	if (UCatalystStatics::GetChat(Player, ChatID, Chat))
	{
		Chat->Client_ReceiveMessage(ChatMessage);
	}
}

void AGameModeCatalyst::BroadcastChatMessage(const FChatMessage& ChatMessage, uint8 ChatID)
{
	for (auto& Player : SessionPlayers)
	{
		SendChatMessageToPlayer(Player, ChatMessage, ChatID);
	}
}

void AGameModeCatalyst::BroadcastChatMessageToPlayers(const TArray<APlayerController*>& Players, const FChatMessage& ChatMessage, uint8 ChatID)
{
	for (auto& Player : Players)
	{
		SendChatMessageToPlayer(Player, ChatMessage, ChatID);
	}
}

void AGameModeCatalyst::BeginPlay()
{
	Super::BeginPlay();

	ServerSubsystem = GetGameInstance()->GetSubsystem<UServerCatalystSubsystem>();

	// Retrieve level spawners
	TArray<AActor*> SpawnerActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerSpawner::StaticClass(), SpawnerActors);
	for (auto A : SpawnerActors)
	{
		if (auto Spawner = Cast<APlayerSpawner>(A))
		{
			PlayerSpawners.Add(Spawner);
		}
	}
}

void AGameModeCatalyst::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (auto Player = Cast<APlayerControllerCatalyst>(NewPlayer))
	{
		SessionPlayers.Add(Player);
		LoginNewPlayer(Player);
	}
}

void AGameModeCatalyst::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (auto Player = Cast<APlayerControllerCatalyst>(NewPlayer))
	{
		StartNewPlayer(Player);
	}
}

void AGameModeCatalyst::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (auto Player = Cast<APlayerControllerCatalyst>(Exiting))
	{
		SessionPlayers.Remove(Player);
		LogoutPlayer(Player);
	}
}

void AGameModeCatalyst::LoginNewPlayer(APlayerControllerCatalyst* NewPlayer)
{
	OnLoginNewPlayer(NewPlayer);

	AssignTeamToPlayer(NewPlayer);
}

void AGameModeCatalyst::StartNewPlayer(APlayerControllerCatalyst* NewPlayer)
{
	OnStartNewPlayer(NewPlayer);

}

void AGameModeCatalyst::LogoutPlayer(APlayerControllerCatalyst* Exiting)
{
	OnLogoutPlayer(Exiting);
}

void AGameModeCatalyst::gm_start()
{
	Start();
}

void AGameModeCatalyst::gm_pause()
{
	Pause();
}

void AGameModeCatalyst::gm_resume()
{
	Resume();
}

void AGameModeCatalyst::gm_start_players()
{
	StartPlayers();
}
