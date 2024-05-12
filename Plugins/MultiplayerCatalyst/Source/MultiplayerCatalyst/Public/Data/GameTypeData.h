// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SettingsData.h"
#include "GameTypeData.generated.h"

class UMapData;

USTRUCT(BlueprintType)
struct FGameTypeDataStructure
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	FString Alias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	uint8 MaxPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	uint8 NumTeams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	TSet<UMapData*> Maps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	USettingsData* Settings;
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API UGameTypeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Type Data")
	FGameTypeDataStructure Data;
};
