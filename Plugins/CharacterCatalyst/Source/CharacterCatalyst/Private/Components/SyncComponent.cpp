// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/SyncComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

USyncComponent::USyncComponent() :
	bReplicateActions(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void USyncComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Gets a reference to the owning character
	Refs.Character = Cast<ACharacter>(GetOwner());

	if (Refs.Character)
	{
		bActive = true;

		// Gets a reference to the remaining components
		Refs.Capsule = Refs.Character->GetCapsuleComponent();
		Refs.CharacterMovement = Refs.Character->GetCharacterMovement();
	}
}

void USyncComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USyncComponent::PlayMontage(UAnimMontage* Montage, FMontagePlayParams Params, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_PlayMontage(Montage, Params);
		}
		else
		{
			Server_PlayMontage(Montage, Params);
		}
	}
	OnPlayMontage.Broadcast(Montage, Params);
}

void USyncComponent::SetMovementMode(EMovementMode NewMode, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_SetMovementMode(NewMode);
		}
		else
		{
			Server_SetMovementMode(NewMode);
		}
	}
	Refs.CharacterMovement->SetMovementMode(NewMode);
}

void USyncComponent::SetLocationAndRotation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_SetLocationAndRotation(NewLocation, NewRotation, Params);
		}
		else
		{
			Server_SetLocationAndRotation(NewLocation, NewRotation, Params);
		}
	}
	Refs.Character->SetActorLocationAndRotation(NewLocation, NewRotation, Params.bSweep, nullptr, Params.TeleportType);
}

void USyncComponent::MoveComponent(USceneComponent* Component, FMoveComponentParams Params, uint8 ID, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_MoveComponentAction(EMoveComponentActionType::Move, Component, Params, ID);
		}
		else
		{
			Server_MoveComponentAction(EMoveComponentActionType::Move, Component, Params, ID);
		}
	}
	OnMoveComponent.Broadcast(Component, Params, ID);
}

void USyncComponent::StopMoveComponent(USceneComponent* Component, FMoveComponentParams Params, uint8 ID, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_MoveComponentAction(EMoveComponentActionType::Stop, Component, Params, ID);
		}
		else
		{
			Server_MoveComponentAction(EMoveComponentActionType::Stop, Component, Params, ID);
		}
	}
	OnStopMoveComponent.Broadcast(Component, Params, ID);
}

void USyncComponent::ReturnMoveComponent(USceneComponent* Component, FMoveComponentParams Params, uint8 ID, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (GetOwner()->HasAuthority())
		{
			NetAll_MoveComponentAction(EMoveComponentActionType::Return, Component, Params, ID);
		}
		else
		{
			Server_MoveComponentAction(EMoveComponentActionType::Return, Component, Params, ID);
		}
	}
	OnReturnMoveComponent.Broadcast(Component, Params, ID);
}

void USyncComponent::AddImpulse(FVector Impulse, bool bVelocityChange, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (!GetOwner()->HasAuthority())
		{
			Server_AddImpulse(Impulse, bVelocityChange);
		}
	}
	Refs.CharacterMovement->AddImpulse(Impulse, bVelocityChange);
}

void USyncComponent::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride, bool bReplicate)
{
	if (bReplicate && bReplicateActions)
	{
		if (!GetOwner()->HasAuthority())
		{
			Server_LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
		}
	}
	Refs.Character->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

void USyncComponent::Server_PlayMontage_Implementation(UAnimMontage* Montage, FMontagePlayParams Params)
{
	PlayMontage(Montage, Params, true);
}

void USyncComponent::NetAll_PlayMontage_Implementation(UAnimMontage* Montage, FMontagePlayParams Params)
{
	if (!GetOwner()->HasAuthority())
		PlayMontage(Montage, Params, false);
}

void USyncComponent::Server_SetMovementMode_Implementation(EMovementMode NewMode)
{
	SetMovementMode(NewMode, true);
}

void USyncComponent::NetAll_SetMovementMode_Implementation(EMovementMode NewMode)
{
	if (!GetOwner()->HasAuthority())
		SetMovementMode(NewMode, false);
}

void USyncComponent::Server_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params)
{
	SetLocationAndRotation(NewLocation, NewRotation, Params, true);
}

void USyncComponent::NetAll_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params)
{
	if (!GetOwner()->HasAuthority())
		SetLocationAndRotation(NewLocation, NewRotation, Params, false);
}

void USyncComponent::Server_MoveComponentAction_Implementation(EMoveComponentActionType Action, USceneComponent* Component, FMoveComponentParams Params, uint8 ID)
{
	switch (Action)
	{
	case EMoveComponentActionType::Move:
		MoveComponent(Component, Params, ID, true);
		break;
	case EMoveComponentActionType::Stop:
		StopMoveComponent(Component, Params, ID, true);
		break;
	case EMoveComponentActionType::Return:
		ReturnMoveComponent(Component, Params, ID, true);
		break;
	}
}

void USyncComponent::NetAll_MoveComponentAction_Implementation(EMoveComponentActionType Action, USceneComponent* Component, FMoveComponentParams Params, uint8 ID)
{
	if (GetOwner()->HasAuthority()) return;

	switch (Action)
	{
	case EMoveComponentActionType::Move:
		MoveComponent(Component, Params, ID, false);
		break;
	case EMoveComponentActionType::Stop:
		StopMoveComponent(Component, Params, ID, false);
		break;
	case EMoveComponentActionType::Return:
		ReturnMoveComponent(Component, Params, ID, false);
		break;
	}
}

void USyncComponent::Server_AddImpulse_Implementation(FVector Impulse, bool bVelocityChange)
{
	AddImpulse(Impulse, bVelocityChange);
}

void USyncComponent::Server_LaunchCharacter_Implementation(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride, false);
}

