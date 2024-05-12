// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IntegerAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIntegerAttributeValueChanged, int32, NewValue, int32, Previous);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIntegerAttributeLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIntegerAttributeUnlocked);

/**
 * Component used to represent a replicated integer attribute. Can be used to represent all sorts of attributes like kills, deaths, score, etc...
 */
UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent), Category="Multiplayer Catalyst" )
class MULTIPLAYERCATALYST_API UIntegerAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UIntegerAttributeComponent();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetAttributeValue(int32 NewValue, bool bForceReplication = false);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void OffsetAttributeValue(int32 Offset);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ResetAttribute();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetLock(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void ForceNewValue(int32 NewValue);

	UFUNCTION(BlueprintPure, Category = "Attribute")
	int32 GetAttributeValue() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool IsLocked() const;

public:

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnIntegerAttributeValueChanged OnIntegerAttributeValueChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnIntegerAttributeLocked OnIntegerAttributeLocked;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnIntegerAttributeUnlocked OnIntegerAttributeUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	int32 DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	int32 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	int32 MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TEnumAsByte<ELifetimeCondition> AttributeReplicationCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	bool bStartLocked;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAttributeValue(int32 NewValue);
	void Server_SetAttributeValue_Implementation(int32 NewValue);
	bool Server_SetAttributeValue_Validate(int32 NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_SetAttributeValue(int32 NewValue);
	void NetAll_SetAttributeValue_Implementation(int32 NewValue);

	UFUNCTION()
	void OnRep_AttributeValue();

	UFUNCTION()
	void OnRep_bLocked();

	UPROPERTY(ReplicatedUsing = "OnRep_AttributeValue")
	int32 AttributeValue;

	UPROPERTY(ReplicatedUsing = "OnRep_bLocked")
	bool bLocked;

	UPROPERTY()
	int32 PreviousValue;
};
