// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/ChatComponent.h"
#include "GameModeCatalyst.generated.h"

class APlayerControllerCatalyst;
class UServerCatalystSubsystem;
class UAttributeMapComponent;
class APlayerSpawner;

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API AGameModeCatalyst : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameModeCatalyst();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode Catalyst")
	void OnLoginNewPlayer(APlayerControllerCatalyst* NewPlayer);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode Catalyst")
	void OnStartNewPlayer(APlayerControllerCatalyst* NewPlayer);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode Catalyst")
	void OnLogoutPlayer(APlayerControllerCatalyst* Exiting);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode Catalyst")
	void OnStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode Catalyst")
	void OnPause();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode Catalyst")
	void OnResume();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode Catalyst")
	void OnStartPlayers();

public:
	UFUNCTION(BlueprintPure, Category = "Game Mode Catalyst")
	int32 GetTeamSize(uint8 Team);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	void AssignTeamToPlayer(APlayerControllerCatalyst* Player);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	uint8 GetAvailableTeam();

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void Start();

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void Pause();

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void Resume();

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void StartPlayers();

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void SpawnPlayer(APlayerControllerCatalyst* Player, bool bStarterSpawner = false);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual APlayerSpawner* FindAvailableSpawner(uint8 Team, bool bStarterSpawner = false);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void SendChatMessageToPlayer(APlayerController* Player, const FChatMessage& ChatMessage, uint8 ChatID);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void BroadcastChatMessage(const FChatMessage& ChatMessage, uint8 ChatID);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Catalyst")
	virtual void BroadcastChatMessageToPlayers(const TArray<APlayerController*>& Players, const FChatMessage& ChatMessage, uint8 ChatID);

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void LoginNewPlayer(APlayerControllerCatalyst* NewPlayer);
	virtual void StartNewPlayer(APlayerControllerCatalyst* NewPlayer);
	virtual void LogoutPlayer(APlayerControllerCatalyst* Exiting);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Players");
	TArray<APlayerControllerCatalyst*> SessionPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Players");
	TArray<APlayerSpawner*> PlayerSpawners;

	UPROPERTY(BlueprintReadOnly, Category = "Players");
	UServerCatalystSubsystem* ServerSubsystem;

	UPROPERTY()
	UAttributeMapComponent* AttributeMap;

public:
	UFUNCTION(Exec)
	void gm_start();

	UFUNCTION(Exec)
	void gm_pause();

	UFUNCTION(Exec)
	void gm_resume();

	UFUNCTION(Exec)
	void gm_start_players();
};
