// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SettingsData.generated.h"

USTRUCT(BlueprintType)
struct FSettingOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Option")
	FString Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Option")
	FText Name;
};

USTRUCT(BlueprintType)
struct FSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FString CustomValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<FSettingOption> Options;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	int32 CurrentOption;

	int32 GetValueOptionIndex(FString Value);

	friend uint32 GetTypeHash(const FSetting& Thing);
};

USTRUCT(BlueprintType)
struct FSettingsContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Container")
	TMap<FName, FSetting> Settings;
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API USettingsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Data")
	FSettingsContainer Settings;
};
