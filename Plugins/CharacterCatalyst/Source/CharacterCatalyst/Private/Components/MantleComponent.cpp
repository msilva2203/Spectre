// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/MantleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveVector.h"
#include "KismetAnimationLibrary.h"

UMantleComponent::UMantleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CapsuleProfileName = TEXT("ALS_Character");
}

void UMantleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Gets a reference to the owning character
	Refs.Character = Cast<ACharacter>(GetOwner());

	if (Refs.Character)
	{
		// Gets a reference to the remaining components
		Refs.Capsule = Refs.Character->GetCapsuleComponent();
		Refs.CharacterMovement = Refs.Character->GetCharacterMovement();

		// Gets reference to the sync component
		auto Comp = Refs.Character->GetComponentByClass(USyncComponent::StaticClass());
		Sync = Cast<USyncComponent>(Comp);
		if (Sync)
		{
			bActive = true;
		}
	}
	else bActive = false;

	FActorSpawnParameters SpawnParams;
	TimelineActor = GetWorld()->SpawnActor<ATimelineActor>(TimelineActorSubclass, SpawnParams);
	if (TimelineActor)
	{
		TimelineActor->OnAlphaUpdated.AddDynamic(this, &UMantleComponent::UpdateMantle);
		TimelineActor->OnEnd.AddDynamic(this, &UMantleComponent::EndMantle);
	}
	else bActive = false;
}

void UMantleComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (TimelineActor)
	{
		TimelineActor->K2_DestroyActor();
	}
}

void UMantleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bActive)
	{
		if (bEnableFallingMantle && Refs.CharacterMovement->IsFalling() && Refs.Character->IsPawnControlled())
		{
			if (Refs.CharacterMovement->GetCurrentAcceleration().Length() > 0.0f)
			{
				// Check for mantle here
				CheckMantle(FallingTraceParams);
			}
		}
	}
}

bool UMantleComponent::CheckMantle(FMantleTraceParams TraceParams)
{
	if (!bActive) return false;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	FVector Start;
	FVector End;
	float Radius;

	EMantleType MantleType = EMantleType::Low;
	float MantleHeight;
	FTransform TargetTransform;
	FVector InitialTraceImpactPoint;
	FVector InitialTraceNormal;
	FVector DownTraceLocation;
	UPrimitiveComponent* HitComponent;

	FVector MovementInput = GetMovementInput();
	FVector CapsuleBaseLocation = GetCapsuleBaseLocation(2.0f);

	//***********************************************************************
	// 1. Find a wall or object where the player cannot walk on
	//***********************************************************************
	Start = (CapsuleBaseLocation + (MovementInput * -30.0f)) + FVector(0.0f, 0.0f, (TraceParams.MaxLedgeHeight + TraceParams.MinLedgeHeight) / 2.0f);
	End = Start + (MovementInput * TraceParams.ReachDistance);
	Radius = TraceParams.ForwardTraceRadius;
	float HalfHeight = (TraceParams.MaxLedgeHeight - TraceParams.MinLedgeHeight) / 2.0f + 1.0f;
	UKismetSystemLibrary::CapsuleTraceSingle(this, Start, End, Radius, HalfHeight, TraceType, false, ActorsToIgnore, DebugType, HitResult, true);

	if (!(HitResult.IsValidBlockingHit() && !(Refs.CharacterMovement->IsWalkable(HitResult))))
	{
		return false;
	}
	InitialTraceImpactPoint = HitResult.ImpactPoint;
	InitialTraceNormal = HitResult.ImpactNormal;

	//***********************************************************************
	// 2. Trace downward to check if location is walkable
	//***********************************************************************
	End = FVector(InitialTraceImpactPoint.X, InitialTraceImpactPoint.Y, CapsuleBaseLocation.Z) + (InitialTraceNormal * -15.0f);
	Start = End + FVector(0.0f, 0.0f, TraceParams.MaxLedgeHeight + TraceParams.DownwardTraceRadius + 1.0f);
	Radius = TraceParams.DownwardTraceRadius;
	UKismetSystemLibrary::SphereTraceSingle(this, Start, End, Radius, TraceType, false, ActorsToIgnore, DebugType, HitResult, true);

	if (!(HitResult.bBlockingHit && Refs.CharacterMovement->IsWalkable(HitResult)))
	{
		return false;
	}
	DownTraceLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
	HitComponent = HitResult.GetComponent();

	//***********************************************************************
	// 3. Check if capsule has space to walk on the location and set mantle transform 
	//***********************************************************************
	FVector GroundCapsuleLocation = GetCapsuleLocationFromBase(DownTraceLocation, 2.0f);
	if (!CapsuleHasRoom(GroundCapsuleLocation, 0.0f, 0.0f))
	{
		return false;
	}
	FRotator TransformRotation = UKismetMathLibrary::MakeRotFromX(InitialTraceNormal * FVector(-1.0f, -1.0f, 0.0f));
	TargetTransform = UKismetMathLibrary::MakeTransform(GroundCapsuleLocation, TransformRotation);
	MantleHeight = GroundCapsuleLocation.Z - Refs.Character->GetActorLocation().Z;

	//***********************************************************************
	// 4. Determine the type of mantle
	//***********************************************************************
	if (Refs.CharacterMovement->IsFalling())
	{
		MantleType = EMantleType::Catch;
	}
	else
	{
		if (MantleHeight > 125.0f)
		{
			MantleType = EMantleType::High;
		}
		else
		{
			MantleType = EMantleType::Low;
		}
	}

	//***********************************************************************
	// 5. Start the mantle
	//***********************************************************************
	FMantleLedge MantleLedge;
	MantleLedge.LedgeTransform = TargetTransform;
	MantleLedge.LedgeComponent = HitComponent;
	StartMantle(MantleHeight, MantleLedge, MantleType);
	return true;
}

bool UMantleComponent::CheckGroundMantle()
{
	return CheckMantle(GroundTraceParams);
}

bool UMantleComponent::CheckFallingMantle()
{
	return CheckMantle(FallingTraceParams);
}

void UMantleComponent::StartMantle(float MantleHeight, FMantleLedge MantleLedge, EMantleType MantleType)
{
	if (!bActive) return;

	OnMantleStart.Broadcast(MantleType);

	//***********************************************************************
	// 1. Get mantle asset and use it to get mantle params
	//***********************************************************************
	FMantleAsset MantleAsset = GetMantleAsset(MantleType);
	MantleParams.Montage = MantleAsset.Montage;
	MantleParams.CorrectionCurve = MantleAsset.CorrectionCurve;
	MantleParams.StartingOffset = MantleAsset.StartingOffset;
	MantleParams.PlayRate = UKismetMathLibrary::MapRangeClamped(MantleHeight, MantleAsset.LowHeight, MantleAsset.HighHeight, MantleAsset.LowPlayRate, MantleAsset.HighPlayRate);
	MantleParams.StartingPosition = UKismetMathLibrary::MapRangeClamped(MantleHeight, MantleAsset.LowHeight, MantleAsset.HighHeight, MantleAsset.LowStartPosition, MantleAsset.HighStartPosition);

	//***********************************************************************
	// 2. Convert ledge to local space
	//***********************************************************************
	MantleLedgeLocal = ML_WorldToLocal(MantleLedge);

	//***********************************************************************
	// 3. Set the mantle target and starting offset
	//    (offset amount between character and target)
	//***********************************************************************
	MantleTarget = MantleLedge.LedgeTransform;
	StartOffset = ML_Transform_Transform(Refs.Character->GetActorTransform(), MantleTarget);

	//***********************************************************************
	// 4. Calculate the animated start offset from the target location
	//    * This would be the location where the animation starts relative to
	//      target transform
	//***********************************************************************
	FVector Temp = (MantleTarget.Rotator().Vector()) * MantleParams.StartingOffset.Y; // TODO: Verifiy this part
	FVector ATranslation = MantleTarget.GetTranslation() - (UKismetMathLibrary::MakeVector(Temp.X, Temp.Y, MantleParams.StartingOffset.Z));
	FTransform A = UKismetMathLibrary::MakeTransform(ATranslation, MantleTarget.Rotator());
	AnimatedStartOffset = ML_Transform_Transform(A, MantleTarget);

	//***********************************************************************
	// 5. Clear the movement mode
	//***********************************************************************
	Sync->SetMovementMode(EMovementMode::MOVE_None, true);

	//***********************************************************************
	// 6. Configure and play the timeline
	//    * same length as the correction curve
	//    * same play rate as the animation
	//***********************************************************************
	UTimelineComponent* Timeline;
	TimelineActor->GetTimeline(Timeline);
	if (Timeline)
	{
		float MinTime, MaxTime;
		MantleParams.CorrectionCurve->GetTimeRange(MinTime, MaxTime);
		Timeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
		Timeline->SetPlayRate(MantleParams.PlayRate);
		Timeline->PlayFromStart();
	}

	//***********************************************************************
	// 7. Play the montage if valid
	//***********************************************************************
	if (MantleAsset.Montage)
	{
		FMontagePlayParams MontageParams;
		MontageParams.PlayRate = MantleParams.PlayRate;
		MontageParams.StartingPosition = MantleParams.StartingPosition;
		Sync->PlayMontage(MantleAsset.Montage, MontageParams, true);
	}
}

void UMantleComponent::UpdateMantle(float Alpha)
{
	if (!bActive) return;

	float PositionAlpha = 0.0f;
	float XY_CorrectionAlpha = 0.0f;
	float Z_CorrectionAlpha = 0.0f;
	FTransform LerpedTarget;

	//***********************************************************************
	// 1. Update mantle target to keep up with moving objects
	//***********************************************************************
	MantleTarget = ML_LocalToWorld(MantleLedgeLocal).LedgeTransform;

	//***********************************************************************
	// 2. Update the position and correction alphas
	//***********************************************************************
	UTimelineComponent* Timeline;
	TimelineActor->GetTimeline(Timeline);
	if (Timeline)
	{
		float Time = Timeline->GetPlaybackPosition() + MantleParams.StartingPosition;
		FVector Value = MantleParams.CorrectionCurve->GetVectorValue(Time);

		PositionAlpha = Value.X;
		XY_CorrectionAlpha = Value.Y;
		Z_CorrectionAlpha = Value.Z;
	}
	else return;

	//***********************************************************************
	// 3. Lerp the transforms together
	//***********************************************************************
	FTransform Comb;
	FVector XYv, Zv;
	FRotator XYr, Zr;

	XYv = UKismetMathLibrary::MakeVector(AnimatedStartOffset.GetTranslation().X, AnimatedStartOffset.GetTranslation().Y, StartOffset.GetTranslation().Z);
	XYr = AnimatedStartOffset.Rotator();

	Zv = UKismetMathLibrary::MakeVector(StartOffset.GetTranslation().X, StartOffset.GetTranslation().Y, AnimatedStartOffset.GetTranslation().Z);
	Zr = StartOffset.Rotator();

	XYv = FMath::Lerp<FVector>(StartOffset.GetTranslation(), XYv, XY_CorrectionAlpha);
	XYr = FMath::Lerp<FRotator>(AnimatedStartOffset.Rotator(), XYr, XY_CorrectionAlpha);

	Zv = FMath::Lerp<FVector>(StartOffset.GetTranslation(), Zv, Z_CorrectionAlpha);
	Zr = FMath::Lerp<FRotator>(StartOffset.Rotator(), Zr, Z_CorrectionAlpha);

	Comb = UKismetMathLibrary::MakeTransform(UKismetMathLibrary::MakeVector(XYv.X, XYv.Y, Zv.Z), XYr);

	Comb = ML_Transform_Plus_Transform(MantleTarget, Comb);
	FVector CombV = Comb.GetTranslation();
	FRotator CombR = Comb.Rotator();

	CombV = FMath::Lerp<FVector>(CombV, MantleTarget.GetTranslation(), PositionAlpha);
	CombR = FMath::Lerp<FRotator>(CombR, MantleTarget.Rotator(), PositionAlpha);

	Comb = ML_Transform_Plus_Transform(MantleTarget, StartOffset);
	CombV = FMath::Lerp<FVector>(Comb.GetTranslation(), CombV, Alpha);
	CombR = FMath::Lerp<FRotator>(Comb.Rotator(), CombR, Alpha);
	LerpedTarget = UKismetMathLibrary::MakeTransform(CombV, CombR);

	//***********************************************************************
	// 4. Set the character location and rotation to the lerped target
	//***********************************************************************
	FTransformUpdateParams UpdateParams;
	UpdateParams.bSweep = false;
	UpdateParams.TeleportType = ETeleportType::None;
	Sync->SetLocationAndRotation(LerpedTarget.GetTranslation(), LerpedTarget.Rotator(), UpdateParams);

}

void UMantleComponent::EndMantle()
{
	if (!bActive) return;

	Sync->SetMovementMode(EMovementMode::MOVE_Walking, true);
	OnMantleEnd.Broadcast();
}

FMantleAsset UMantleComponent::GetMantleAsset(EMantleType MantleType)
{
	bool bRandom = false;
	switch (MantleType)
	{
	case EMantleType::Low:
		bRandom = UKismetMathLibrary::RandomBool();
		if (bRandom) return LowMantleAsset_LH;
		return LowMantleAsset_RH;
	case EMantleType::High:
	case EMantleType::Catch:
		return HighMantleAsset;
	}
	return LowMantleAsset_RH;
}

FVector UMantleComponent::GetCapsuleBaseLocation(float ZOffset)
{
	FVector Out = Refs.Capsule->GetComponentLocation();
	Out = Out - (Refs.Capsule->GetUpVector() * (Refs.Capsule->GetScaledCapsuleHalfHeight() + ZOffset));
	return Out;
}

FVector UMantleComponent::GetMovementInput()
{
	FRotator CharacterRotation = Refs.Character->GetActorRotation();
	FRotator InputRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + GetLastDirection(), CharacterRotation.Roll);
	FVector Out = UKismetMathLibrary::GetForwardVector(InputRotation);
	return Out;
}

float UMantleComponent::GetLastDirection()
{
	return UKismetAnimationLibrary::CalculateDirection(Refs.CharacterMovement->GetLastInputVector(), Refs.Character->GetActorRotation());
}

FVector UMantleComponent::GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset)
{
	FVector Out = BaseLocation + FVector(0.0f, 0.0f, Refs.Capsule->GetScaledCapsuleHalfHeight() + ZOffset);
	return Out;
}

bool UMantleComponent::CapsuleHasRoom(FVector TargetLocation, float RadiusOffset, float HeightOffset)
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	FVector Offset = FVector(0.0f, 0.0f, Refs.Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() + (RadiusOffset * -1.0f) + HeightOffset);
	FVector Start = TargetLocation + Offset;
	FVector End = TargetLocation - Offset;
	float Radius = Refs.Capsule->GetScaledCapsuleRadius() + RadiusOffset;
	UKismetSystemLibrary::SphereTraceSingleByProfile(this, Start, End, Radius, CapsuleProfileName, false, ActorsToIgnore, DebugType, HitResult, true);

	return !(HitResult.bBlockingHit) && !(HitResult.IsValidBlockingHit());
}

FMantleLedge UMantleComponent::ML_WorldToLocal(FMantleLedge MantleLedge)
{
	MantleLedge.LedgeTransform = MantleLedge.LedgeTransform * UKismetMathLibrary::InvertTransform(MantleLedge.LedgeComponent->GetComponentTransform());
	return MantleLedge;
}

FMantleLedge UMantleComponent::ML_LocalToWorld(FMantleLedge MantleLedge)
{
	FTransform T = UKismetMathLibrary::InvertTransform(MantleLedge.LedgeComponent->GetComponentTransform());
	FVector Translation = UKismetMathLibrary::InverseTransformLocation(T, MantleLedge.LedgeTransform.GetTranslation());
	FRotator Rotation = UKismetMathLibrary::InverseTransformRotation(T, MantleLedge.LedgeTransform.Rotator());
	FVector Scale = UKismetMathLibrary::InverseTransformLocation(T, MantleLedge.LedgeTransform.GetScale3D());
	MantleLedge.LedgeTransform = UKismetMathLibrary::MakeTransform(Translation, Rotation, Scale);
	return MantleLedge;
}

FTransform UMantleComponent::ML_Transform_Transform(FTransform A, FTransform B)
{
	FVector Translation = A.GetTranslation() - B.GetTranslation();
	FVector Scale = A.GetScale3D() - B.GetScale3D();
	FRotator Rotation;
	Rotation.Roll = A.Rotator().Roll - B.Rotator().Roll;
	Rotation.Pitch = A.Rotator().Pitch - B.Rotator().Pitch;
	Rotation.Yaw = A.Rotator().Yaw - B.Rotator().Yaw;
	FTransform Out = UKismetMathLibrary::MakeTransform(Translation, Rotation, Scale);
	return Out;
}

FTransform UMantleComponent::ML_Transform_Plus_Transform(FTransform A, FTransform B)
{
	FVector Translation = A.GetTranslation() + B.GetTranslation();
	FVector Scale = A.GetScale3D() + B.GetScale3D();
	FRotator Rotation;
	Rotation.Roll = A.Rotator().Roll + B.Rotator().Roll;
	Rotation.Pitch = A.Rotator().Pitch + B.Rotator().Pitch;
	Rotation.Yaw = A.Rotator().Yaw + B.Rotator().Yaw;
	FTransform Out = UKismetMathLibrary::MakeTransform(Translation, Rotation, Scale);
	return Out;
}

