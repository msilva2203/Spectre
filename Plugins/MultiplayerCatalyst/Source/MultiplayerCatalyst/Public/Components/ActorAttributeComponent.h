// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorAttributeChanged, AActor*, NewActor, AActor*, Previous);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorAttributeLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorAttributeUnlocked);


UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERCATALYST_API UActorAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UActorAttributeComponent();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Actor Attribute")
	void SetActor(AActor* NewActor, bool bForceReplication = false);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetLock(bool bNewValue);

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool IsLocked() const;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnActorAttributeChanged OnActorAttributeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnActorAttributeLocked OnActorAttributeLocked;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnActorAttributeUnlocked OnActorAttributeUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TSubclassOf<AActor> ActorSubclass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TEnumAsByte<ELifetimeCondition> AttributeReplicationCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	bool bStartLocked;

private:
	UFUNCTION(NetMulticast, Reliable)
	void NetAll_SetActor(AActor* NewActor);
	void NetAll_SetActor_Implementation(AActor* NewActor);

	UFUNCTION()
	void OnRep_Actor();

	UFUNCTION()
	void OnRep_bLocked();

	UPROPERTY(ReplicatedUsing = "OnRep_Actor")
	AActor* Actor;

	UPROPERTY(ReplicatedUsing = "OnRep_bLocked")
	bool bLocked;

	UPROPERTY()
	AActor* Previous;
};
