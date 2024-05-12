// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SessionCatalystSubsystem.generated.h"

class APlayerStateCatalyst;
class UIntegerAttributeComponent;
class UScalarAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateAdded, APlayerStateCatalyst*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateRemoved, APlayerStateCatalyst*, PlayerState);

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API USessionCatalystSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void AddPlayer(APlayerStateCatalyst* NewPlayerState);
	void RemovePlayer(APlayerStateCatalyst* Removing);

	UPROPERTY(BlueprintAssignable, Category = "Session Events")
	FOnPlayerStateAdded OnPlayerStateAdded;

	UPROPERTY(BlueprintAssignable, Category = "Session Events")
	FOnPlayerStateRemoved OnPlayerStateRemoved;

	UPROPERTY(BlueprintReadOnly, Category = "Session Players")
	TArray<APlayerStateCatalyst*> SessionPlayerStates;
};
