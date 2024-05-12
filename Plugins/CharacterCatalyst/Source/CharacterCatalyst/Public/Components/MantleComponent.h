// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SyncComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TimelineComponent.h"
#include "Actors/TimelineActor.h"
#include "MantleComponent.generated.h"

UENUM(BlueprintType)
enum class EMantleType : uint8
{
	Low              UMETA(DisplayName = "LOW"),
	High             UMETA(DisplayName = "HIGH"),
	Catch            UMETA(DisplayName = "CATCH")
};

USTRUCT(BlueprintType)
struct FMantleTraceParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Trace Params")
	float MaxLedgeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Trace Params")
	float MinLedgeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Trace Params")
	float ReachDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Trace Params")
	float ForwardTraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Trace Params")
	float DownwardTraceRadius;
};

USTRUCT(BlueprintType)
struct FMantleLedge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Ledge")
	FTransform LedgeTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Ledge")
	UPrimitiveComponent* LedgeComponent;
};

USTRUCT(BlueprintType)
struct FMantleAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	UCurveVector* CorrectionCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	FVector StartingOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	float LowHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	float LowPlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	float LowStartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	float HighHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	float HighPlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Asset")
	float HighStartPosition;
};

USTRUCT(BlueprintType)
struct FMantleParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Params")
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Params")
	UCurveVector* CorrectionCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Params")
	float StartingPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Params")
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Params")
	FVector StartingOffset;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMantleStart, EMantleType, MantleType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMantleEnd);


UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent) )
class CHARACTERCATALYST_API UMantleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMantleComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Mantle")
	bool CheckMantle(FMantleTraceParams TraceParams);

	UFUNCTION(BlueprintCallable, Category = "Mantle")
	bool CheckGroundMantle();

	UFUNCTION(BlueprintCallable, Category = "Mantle")
	bool CheckFallingMantle();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	FMantleAsset HighMantleAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	FMantleAsset LowMantleAsset_RH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	FMantleAsset LowMantleAsset_LH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Config")
	TSubclassOf<ATimelineActor> TimelineActorSubclass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Config")
	TEnumAsByte<ETraceTypeQuery> TraceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Config")
	bool bEnableFallingMantle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Config")
	FMantleTraceParams GroundTraceParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Config")
	FMantleTraceParams FallingTraceParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Config")
	FName CapsuleProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(BlueprintAssignable, Category = "Mantle")
	FOnMantleStart OnMantleStart;

	UPROPERTY(BlueprintAssignable, Category = "Mantle")
	FOnMantleEnd OnMantleEnd;

private:
	UPROPERTY()
	FBaseCharacterReferences Refs;

	UPROPERTY()
	USyncComponent* Sync;

	UPROPERTY()
	bool bActive;

	UPROPERTY()
	FMantleLedge MantleLedgeLocal;

	UPROPERTY()
	FMantleParams MantleParams;

	UPROPERTY()
	FTransform AnimatedStartOffset;

	UPROPERTY()
	FTransform StartOffset;

	UPROPERTY()
	FTransform MantleTarget;

	UPROPERTY()
	ATimelineActor* TimelineActor;

private:
	UFUNCTION()
	void StartMantle(float MantleHeight, FMantleLedge MantleLedge, EMantleType MantleType);

	UFUNCTION()
	void UpdateMantle(float Alpha);

	UFUNCTION()
	void EndMantle();

	UFUNCTION()
	FMantleAsset GetMantleAsset(EMantleType MantleType);

	UFUNCTION()
	FVector GetCapsuleBaseLocation(float ZOffset);

	UFUNCTION()
	FVector GetMovementInput();

	UFUNCTION()
	float GetLastDirection();

	UFUNCTION()
	FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset);

	UFUNCTION()
	bool CapsuleHasRoom(FVector TargetLocation, float RadiusOffset, float HeightOffset);

	static FMantleLedge ML_WorldToLocal(FMantleLedge MantleLedge);
	static FMantleLedge ML_LocalToWorld(FMantleLedge MantleLedge);
	static FTransform ML_Transform_Transform(FTransform A, FTransform B);
	static FTransform ML_Transform_Plus_Transform(FTransform A, FTransform B);
};
