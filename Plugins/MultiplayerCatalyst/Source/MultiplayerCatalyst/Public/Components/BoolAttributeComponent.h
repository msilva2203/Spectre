// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoolAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoolAttributeValueChanged, bool, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoolAttributeLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoolAttributeUnlocked);


UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERCATALYST_API UBoolAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UBoolAttributeComponent();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetAttributeValue(bool NewValue, bool bForceReplication = false);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ResetAttribute();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetLock(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ForceNewValue(bool NewValue);

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool GetAttributeValue() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool IsLocked() const;

public:

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnBoolAttributeValueChanged OnBoolAttributeValueChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnBoolAttributeLocked OnBoolAttributeLocked;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnBoolAttributeUnlocked OnBoolAttributeUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	bool DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TEnumAsByte<ELifetimeCondition> AttributeReplicationCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	bool bStartLocked;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAttributeValue(bool NewValue);
	void Server_SetAttributeValue_Implementation(bool NewValue);
	bool Server_SetAttributeValue_Validate(bool NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_SetAttributeValue(bool NewValue);
	void NetAll_SetAttributeValue_Implementation(bool NewValue);

	UFUNCTION()
	void OnRep_AttributeValue();

	UFUNCTION()
	void OnRep_bLocked();

	UPROPERTY(ReplicatedUsing = "OnRep_AttributeValue")
	bool AttributeValue;

	UPROPERTY(ReplicatedUsing = "OnRep_bLocked")
	bool bLocked;
};
