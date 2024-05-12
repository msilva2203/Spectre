// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/StateMachineComponent.h"

#include "Net/UnrealNetwork.h"

UStateMachineComponent::UStateMachineComponent() :
	DefaultState(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

}

void UStateMachineComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStateMachineComponent, State);
}

void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateStateToDefault(false);
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UStateMachineComponent::UpdateState(uint8 NewState, bool bForceReplication)
{
	if (NewState != State && !IsStateLocked(NewState))
	{
		State = NewState;
		if (bForceReplication && !(GetOwner()->HasAuthority()))
		{
			Server_UpdateState(NewState);
		}
		OnRep_State();
	}
}

void UStateMachineComponent::UpdateStateToDefault(bool bForceReplication)
{
	UpdateState(DefaultState, bForceReplication);
}

void UStateMachineComponent::UpdateStateToPrevious(bool bForceReplication)
{
	UpdateState(PreviousState, bForceReplication);
}

bool UStateMachineComponent::LockState(uint8 StateToLock, bool bForceExit)
{
	if (IsStateLocked(StateToLock)) return false; // already locked
	if (StateToLock == GetState())
	{
		if (bForceExit)
		{
			UpdateStateToPrevious(true);
		}
		else return false;
	}
	LockedStates.Add(StateToLock);
	return true;
}

bool UStateMachineComponent::UnlockState(uint8 StateToUnlock)
{
	if (!IsStateLocked(StateToUnlock)) return false; // already unlocked
	LockedStates.Remove(StateToUnlock);
	return true;
}

bool UStateMachineComponent::IsStateLocked(uint8 InState)
{
	for (int i = 0; i < LockedStates.Num(); i++)
	{
		if (LockedStates[i] == InState) return true;
	}
	return false;
}

uint8 UStateMachineComponent::GetState() const
{
	return State;
}

uint8 UStateMachineComponent::GetPreviousState() const
{
	return PreviousState;
}

bool UStateMachineComponent::IsCurrentState(uint8 InState)
{
	return State == InState;
}

void UStateMachineComponent::Server_UpdateState_Implementation(uint8 NewState)
{
	UpdateState(NewState, false);
}

void UStateMachineComponent::OnRep_State()
{
	OnEnterState.Broadcast(State);
	OnLeaveState.Broadcast(TransitionPreviousState);
	OnStateTranstion.Broadcast(TransitionPreviousState, State);

	PreviousState = PreviousState;
	TransitionPreviousState = State;
}

