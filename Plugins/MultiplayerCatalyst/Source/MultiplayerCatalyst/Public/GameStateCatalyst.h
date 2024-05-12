// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateCatalyst.generated.h"

class UAttributeMapComponent;
class UServerCatalystSubsystem;
class UGameTypeData;
class UMapData;
class USettingsContainerComponent;
class UBoolAttributeComponent;

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API AGameStateCatalyst : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGameStateCatalyst();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void BeginPlay() override;

	UPROPERTY()
	UAttributeMapComponent* AttributeMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State Catalyst")
	UBoolAttributeComponent* InProgressAttribute;

	UPROPERTY(BlueprintReadOnly, Category = "Game State Catalyst")
	UServerCatalystSubsystem* ServerSubsystem;

public:

public:
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentGameType", EditAnywhere, BlueprintReadOnly, Category = "Game State Catalyst")
	UGameTypeData* CurrentGameType;

	UPROPERTY(ReplicatedUsing = "OnRep_CurrentMap", EditAnywhere, BlueprintReadOnly, Category = "Game State Catalyst")
	UMapData* CurrentMap;

	UPROPERTY(EditAnywhere, Category =" Game State Catalyst")
	USettingsContainerComponent* GameSettingsContainer;

private:
	UFUNCTION()
	void SetCurrentGameType(UGameTypeData* NewGameType);

	UFUNCTION()
	void SetCurrentMap(UMapData* NewMap);

	UFUNCTION()
	void OnGameSettingsUpdated();

	UFUNCTION()
	void OnRep_CurrentGameType();

	UFUNCTION()
	void OnRep_CurrentMap();
};
