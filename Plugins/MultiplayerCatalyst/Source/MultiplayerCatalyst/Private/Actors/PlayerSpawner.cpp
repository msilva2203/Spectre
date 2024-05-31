// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Actors/PlayerSpawner.h"
#include "PlayerControllerCatalyst.h"

void APlayerSpawner::SpawnPlayer(APlayerControllerCatalyst* Player)
{
	// Spawning the character
	FActorSpawnParameters SpawnParameters;
	ESpawnActorCollisionHandlingMethod SpawnCollisionMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto NewPawn = GetWorld()->SpawnActorDeferred<APawn>(PawnSubclass, GetActorTransform(), Player, nullptr, SpawnCollisionMethod);

	// Setup character properties and possession
	Player->Possess(NewPawn);
	NativeSetupNewPawn(Player, NewPawn);

	// Finish spawning
	NewPawn->FinishSpawning(GetActorTransform());

	// Notify blueprint actors
	OnPlayerSpawned(Player, NewPawn);
}

void APlayerSpawner::NativeSetupNewPawn(APlayerControllerCatalyst* Player, APawn* NewPawn)
{
	SetupNewPawn(Player, NewPawn);

}
