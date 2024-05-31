// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CatalystActor.h"
#include "PlayerSpawner.generated.h"

class APlayerControllerCatalyst;

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API APlayerSpawner : public ACatalystActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Player Spawner")
	virtual void SpawnPlayer(APlayerControllerCatalyst* Player);

	UFUNCTION()
	virtual void NativeSetupNewPawn(APlayerControllerCatalyst* Player, APawn* NewPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Spawner")
	void SetupNewPawn(APlayerControllerCatalyst* Player, APawn* NewPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Spawner")
	void OnPlayerSpawned(APlayerControllerCatalyst* Player, APawn* NewPawn);

public:
	UPROPERTY(EditAnywhere, Category = "Player Spawner")
	TSubclassOf<APawn> PawnSubclass;

	UPROPERTY(EditAnywhere, Category = "Player Spawner")
	uint8 SpawnerTeam;

	UPROPERTY(EditAnywhere, Category = "Player Spawner")
	bool bStarterSpawner;
};
