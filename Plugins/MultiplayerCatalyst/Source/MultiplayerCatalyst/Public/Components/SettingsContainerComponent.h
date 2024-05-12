// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/SettingsData.h"
#include "SettingsContainerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameSettingsUpdated);


UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERCATALYST_API USettingsContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	USettingsContainerComponent();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Settigns")
	void SetSettings(FSettingsContainer NewSettings);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetSettingOption(FName SettingName, int32 NewOption, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetSettingCustomValue(FName SettingName, const FString& NewValue, bool bReplicate = true);

public:

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	FSettingsContainer Settings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	USettingsData* DefaultSettings;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnGameSettingsUpdated OnGameSettingsUpdated;

private:
	
	UFUNCTION(NetMulticast, Unreliable)
	void NetAll_SetSettingOption(FName SettingName, int32 NewOption);
	void NetAll_SetSettingOption_Implementation(FName SettingName, int32 NewOption);

	UFUNCTION(NetMulticast, Unreliable)
	void NetAll_SetSettingCustomValue(FName SettingName, const FString& NewValue);
	void NetAll_SetSettingCustomValue_Implementation(FName SettingName, const FString& NewValue);

	UPROPERTY(ReplicatedUsing = "OnRep_SettingsValueArray")
	TArray<FSetting> SettingsValueArray;

	UPROPERTY(ReplicatedUsing = "OnRep_SettingsKeyArray")
	TArray<FName> SettingsKeyArray;

	UFUNCTION()
	void OnRep_SettingsValueArray();

	UFUNCTION()
	void OnRep_SettingsKeyArray();

	UFUNCTION()
	void GenerateSettingsContainer();

};
