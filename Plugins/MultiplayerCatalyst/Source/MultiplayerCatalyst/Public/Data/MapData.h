// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapData.generated.h"

USTRUCT(BlueprintType)
struct FMapDataStructure
{
	GENERATED_BODY()

	// The actual name of the level on disk
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	FName LevelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	FText Description;
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API UMapData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	FMapDataStructure Data;
};
