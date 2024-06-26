// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipmentData.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRE_API UEquipmentData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FText Description;
};
