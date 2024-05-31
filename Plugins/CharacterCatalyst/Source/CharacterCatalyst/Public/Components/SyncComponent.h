// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "SyncComponent.generated.h"

USTRUCT(BlueprintType)
struct FBaseCharacterReferences
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character References")
	ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character References")
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character References")
	UCharacterMovementComponent* CharacterMovement;
};

USTRUCT(BlueprintType)
struct FMontagePlayParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage Play")
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage Play")
	float StartingPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage Play")
	FName StartingSection;
};

USTRUCT(BlueprintType)
struct FTransformUpdateParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Update")
	bool bSweep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Update")
	ETeleportType TeleportType;
};

USTRUCT(BlueprintType)
struct FMoveComponentParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Component Params")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Component Params")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Component Params")
	bool bEaseOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Component Params")
	bool bEaseIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Component Params")
	float OverTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Component Params")
	bool bForceShortestRotationPath;
};

UENUM(BlueprintType)
enum class EMoveComponentActionType : uint8
{
	Move, Stop, Return
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayMontage, UAnimMontage*, Montage, FMontagePlayParams, Params);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMoveComponent, USceneComponent*, Component, FMoveComponentParams, Params, uint8, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStopMoveComponent, USceneComponent*, Component, FMoveComponentParams, Params, uint8, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReturnMoveComponent, USceneComponent*, Component, FMoveComponentParams, Params, uint8, ID);


UCLASS(ClassGroup = (Catalyst), meta = (BlueprintSpawnableComponent))
class CHARACTERCATALYST_API USyncComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USyncComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void PlayMontage(UAnimMontage* Montage, FMontagePlayParams Params, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void SetMovementMode(EMovementMode NewMode, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void SetLocationAndRotation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void MoveComponent(USceneComponent* Component, FMoveComponentParams Params, uint8 ID, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void StopMoveComponent(USceneComponent* Component, FMoveComponentParams Params, uint8 ID, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void ReturnMoveComponent(USceneComponent* Component, FMoveComponentParams Params, uint8 ID, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void AddImpulse(FVector Impulse, bool bVelocityChange = true, bool bReplicate = true);

	UFUNCTION(BlueprintCallable, Category = "Sync Component")
	void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride = false, bool bZOverride = false, bool bReplicate = true);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Component")
	bool bReplicateActions;

	UPROPERTY(BlueprintAssignable, Category = "Sync Component")
	FOnPlayMontage OnPlayMontage;

	UPROPERTY(BlueprintAssignable, Category = "Sync Component")
	FOnMoveComponent OnMoveComponent;

	UPROPERTY(BlueprintAssignable, Category = "Sync Component")
	FOnStopMoveComponent OnStopMoveComponent;

	UPROPERTY(BlueprintAssignable, Category = "Sync Component")
	FOnReturnMoveComponent OnReturnMoveComponent;

private:
	UPROPERTY()
	FBaseCharacterReferences Refs;

	UPROPERTY()
	bool bActive;

private:

	UFUNCTION(Server, Reliable)
	void Server_PlayMontage(UAnimMontage* Montage, FMontagePlayParams Params);
	void Server_PlayMontage_Implementation(UAnimMontage* Montage, FMontagePlayParams Params);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_PlayMontage(UAnimMontage* Montage, FMontagePlayParams Params);
	void NetAll_PlayMontage_Implementation(UAnimMontage* Montage, FMontagePlayParams Params);

	UFUNCTION(Server, Reliable)
	void Server_SetMovementMode(EMovementMode NewMode);
	void Server_SetMovementMode_Implementation(EMovementMode NewMode);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_SetMovementMode(EMovementMode NewMode);
	void NetAll_SetMovementMode_Implementation(EMovementMode NewMode);

	UFUNCTION(Server, Reliable)
	void Server_SetLocationAndRotation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params);
	void Server_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_SetLocationAndRotation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params);
	void NetAll_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation, FTransformUpdateParams Params);

	UFUNCTION(Server, Reliable)
	void Server_MoveComponentAction(EMoveComponentActionType Action, USceneComponent* Component, FMoveComponentParams Params, uint8 ID);
	void Server_MoveComponentAction_Implementation(EMoveComponentActionType Action, USceneComponent* Component, FMoveComponentParams Params, uint8 ID);

	UFUNCTION(NetMulticast, Reliable)
	void NetAll_MoveComponentAction(EMoveComponentActionType Action, USceneComponent* Component, FMoveComponentParams Params, uint8 ID);
	void NetAll_MoveComponentAction_Implementation(EMoveComponentActionType Action, USceneComponent* Component, FMoveComponentParams Params, uint8 ID);

	UFUNCTION(Server, Reliable)
	void Server_AddImpulse(FVector Impulse, bool bVelocityChange);
	void Server_AddImpulse_Implementation(FVector Impulse, bool bVelocityChange);
	
	UFUNCTION(Server, Reliable)
	void Server_LaunchCharacter(FVector LaunchVelocity, bool bXYOverride = false, bool bZOverride = false);
	void Server_LaunchCharacter_Implementation(FVector LaunchVelocity, bool bXYOverride = false, bool bZOverride = false);
};
