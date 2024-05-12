// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ByteAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnByteAttributeValueChanged, uint8, NewValue, uint8, Previous);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnByteAttributeLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnByteAttributeUnlocked);

/**
 * Component used to represent a replicated byte attribute. Can be used to represent all sorts of attributes like leading team, etc...
 */
UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERCATALYST_API UByteAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UByteAttributeComponent();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetAttributeValue(uint8 NewValue, bool bForceReplication = false);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void OffsetAttributeValue(uint8 Offset);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ResetAttribute();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetLock(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ForceNewValue(uint8 NewValue);

	UFUNCTION(BlueprintPure, Category = "Attribute")
	uint8 GetAttributeValue() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool IsLocked() const;

public:

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnByteAttributeValueChanged OnByteAttributeValueChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnByteAttributeLocked OnByteAttributeLocked;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnByteAttributeUnlocked OnByteAttributeUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	uint8 DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	uint8 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	uint8 MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TEnumAsByte<ELifetimeCondition> AttributeReplicationCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	bool bStartLocked;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAttributeValue(uint8 NewValue);
	void Server_SetAttributeValue_Implementation(uint8 NewValue);
	bool Server_SetAttributeValue_Validate(uint8 NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_SetAttributeValue(uint8 NewValue);
	void NetAll_SetAttributeValue_Implementation(uint8 NewValue);

	UFUNCTION()
	void OnRep_AttributeValue();

	UFUNCTION()
	void OnRep_bLocked();

	UPROPERTY(ReplicatedUsing = "OnRep_AttributeValue")
	uint8 AttributeValue;

	UPROPERTY(ReplicatedUsing = "OnRep_bLocked")
	bool bLocked;

	UPROPERTY()
	uint8 PreviousValue;
};
