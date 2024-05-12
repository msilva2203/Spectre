// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimelineActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlphaUpdated, float, NewAlpha);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnd);

UCLASS()
class CHARACTERCATALYST_API ATimelineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATimelineActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Timeline Actor")
	void NotifyAlpha(const float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Timeline Actor")
	void NotifyEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Timeline Actor")
	void GetTimeline(UTimelineComponent*& OutTimeline);

	UPROPERTY(BlueprintAssignable, Category = "Timeline Actor")
	FOnAlphaUpdated OnAlphaUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Timeline Actor")
	FOnEnd OnEnd;
};
