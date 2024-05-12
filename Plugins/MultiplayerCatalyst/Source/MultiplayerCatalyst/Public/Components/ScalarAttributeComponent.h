// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScalarAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScalarAttributeValueChanged, float, NewValue, float, Previous);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScalarAttributeLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScalarAttributeUnlocked);

/**
 * Component used to represent a replicated scalar attribute. Can be used to represent all sorts of attributes like health, stamina, etc...
 */
UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent), Category = "Multiplayer Catalyst" )
class MULTIPLAYERCATALYST_API UScalarAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UScalarAttributeComponent();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetAttributeValue(float NewValue, bool bForceReplication = false);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void OffsetAttributeValue(float Offset);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ResetAttribute();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetLock(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ForceNewValue(float NewValue);

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetAttributeValue() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool IsLocked() const;

public:

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnScalarAttributeValueChanged OnScalarAttributeValueChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnScalarAttributeLocked OnScalarAttributeLocked;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnScalarAttributeUnlocked OnScalarAttributeUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	float DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	float MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	float MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	TEnumAsByte<ELifetimeCondition> AttributeReplicationCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	bool bStartLocked;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAttributeValue(float NewValue);
	void Server_SetAttributeValue_Implementation(float NewValue);
	bool Server_SetAttributeValue_Validate(float NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_SetAttributeValue(float NewValue);
	void NetAll_SetAttributeValue_Implementation(float NewValue);

	UFUNCTION()
	void OnRep_AttributeValue();

	UFUNCTION()
	void OnRep_bLocked();

	UPROPERTY(ReplicatedUsing = "OnRep_AttributeValue")
	float AttributeValue;

	UPROPERTY(ReplicatedUsing = "OnRep_bLocked")
	bool bLocked;

	UPROPERTY()
	float PreviousValue;
};
