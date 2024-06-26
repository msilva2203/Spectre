// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionBufferComponent.generated.h"

USTRUCT(BlueprintType)
struct FBufferAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffer Action")
	bool bActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffer Action")
	float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffer Action")
	FTimerHandle TimerHandle;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBufferEndDelegate, FName, BufferName);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERCATALYST_API UActionBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionBufferComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Action Buffer")
	void SetBuffer(const FName BufferName, float Time);

	UFUNCTION(BlueprintCallable, Category = "Action Buffer")
	void ClearBuffer(const FName BufferName);

	UFUNCTION(BlueprintCallable, Category = "Action Buffer")
	bool IsBufferActive(const FName BufferName);

	UPROPERTY(BlueprintAssignable, Category = "Action Buffer")
	FOnBufferEndDelegate OnBufferEnd;

	UFUNCTION()
	void OnBufferUpdate();

private:

	UPROPERTY()
	TMap<FName, FBufferAction> BufferMap;
};
