// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Actors/TimelineActor.h"

ATimelineActor::ATimelineActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATimelineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimelineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimelineActor::NotifyAlpha(const float Alpha)
{
	OnAlphaUpdated.Broadcast(Alpha);
}

void ATimelineActor::NotifyEnd()
{
	OnEnd.Broadcast();
}

