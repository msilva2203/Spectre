// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterState, uint8, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeaveState, uint8, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateTranstion, uint8, PreviousState, uint8, NewState);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERCATALYST_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateMachineComponent();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void UpdateState(uint8 NewState, bool bForceReplication = true);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void UpdateStateToDefault(bool bForceReplication = true);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void UpdateStateToPrevious(bool bForceReplication = true);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	bool LockState(uint8 StateToLock, bool bForceExit = true);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	bool UnlockState(uint8 StateToUnlock);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	bool IsStateLocked(uint8 State);

	UFUNCTION(BlueprintPure, Category = "State Machine")
	uint8 GetState() const;

	UFUNCTION(BlueprintPure, Category = "State Machine")
	uint8 GetPreviousState() const;

	UFUNCTION(BlueprintPure, Category = "State Machine")
	bool IsCurrentState(uint8 InState);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Machine")
	uint8 DefaultState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Machine")
	TArray<uint8> LockedStates;

	UPROPERTY(BlueprintAssignable, Category = "State Machine")
	FOnEnterState OnEnterState;

	UPROPERTY(BlueprintAssignable, Category = "State Machine")
	FOnLeaveState OnLeaveState;

	UPROPERTY(BlueprintAssignable, Category = "State Machine")
	FOnStateTranstion OnStateTranstion;

private:
	UPROPERTY(ReplicatedUsing = "OnRep_State");
	uint8 State;

	UPROPERTY()
	uint8 PreviousState;

	UPROPERTY()
	uint8 TransitionPreviousState;

private:
	UFUNCTION(Server, Reliable)
	void Server_UpdateState(uint8 NewState);
	void Server_UpdateState_Implementation(uint8 NewState);

	UFUNCTION()
	void OnRep_State();
};
