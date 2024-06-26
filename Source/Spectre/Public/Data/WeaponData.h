// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

class UEquipmentData;

/**
 * 
 */
UCLASS()
class SPECTRE_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UEquipmentData* Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float BurstRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 BurstAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsAutomatic;

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetEffectiveRate() const;
};
