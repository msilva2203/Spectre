// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/CatalystInterface.h"
#include "PlayerStateCatalyst.generated.h"

class USessionCatalystSubsystem;
class UAttributeMapComponent;
class UByteAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTeamUpdated, uint8, NewTeam);

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API APlayerStateCatalyst : public APlayerState, public ICatalystInterface
{
	GENERATED_BODY()
	
public:
	APlayerStateCatalyst();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void CopyProperties(APlayerState* PlayerState) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual uint8 GetTeam() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Catalyst")
	UByteAttributeComponent* PlayerTeamAttribute;

	UPROPERTY(BlueprintAssignable, Category = "Catalyst")
	FOnPlayerTeamUpdated OnPlayerTeamUpdated;

	UPROPERTY(BlueprintReadOnly, Category = "Subsystems")
	USessionCatalystSubsystem* SessionSubsystem;

	UPROPERTY()
	UAttributeMapComponent* AttributeMap;

private:
	virtual void OnRep_PlayerName() override;

public:
	UFUNCTION(Exec)
	virtual void ps_set_player_team(uint8 new_team);
	UFUNCTION(Exec)
	virtual void ps_set_int_attribute(FName name, int32 new_value);
};
