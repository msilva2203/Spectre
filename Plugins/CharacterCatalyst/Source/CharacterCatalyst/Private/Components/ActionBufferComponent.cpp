// © 2024 Marco Silva. All Rights Reserved.


#include "Components/ActionBufferComponent.h"

// Sets default values for this component's properties
UActionBufferComponent::UActionBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActionBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionBufferComponent::SetBuffer(const FName BufferName, float Time)
{
	auto Buffer = BufferMap.Find(BufferName);

	if (Buffer)
	{
		Buffer->bActive = true;
		Buffer->Time = Time;
		GetWorld()->GetTimerManager().ClearTimer(Buffer->TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(Buffer->TimerHandle, 
			this, 
			&UActionBufferComponent::OnBufferUpdate, 
			Time, 
			false);
	}
	else
	{
		FBufferAction NewBuffer;
		NewBuffer.bActive = true;
		NewBuffer.Time = Time;
		GetWorld()->GetTimerManager().SetTimer(NewBuffer.TimerHandle,
			this,
			&UActionBufferComponent::OnBufferUpdate,
			Time,
			false);

		BufferMap.Add(BufferName, NewBuffer);
	}
}

void UActionBufferComponent::ClearBuffer(const FName BufferName)
{
	auto Buffer = BufferMap.Find(BufferName);

	if (Buffer)
	{
		GetWorld()->GetTimerManager().ClearTimer(Buffer->TimerHandle);
		Buffer->bActive = false;
	}
}

bool UActionBufferComponent::IsBufferActive(const FName BufferName)
{
	auto Buffer = BufferMap.Find(BufferName);

	if (Buffer)
	{
		return Buffer->bActive;
	}
	return false;
}

void UActionBufferComponent::OnBufferUpdate()
{
	GetWorld()->GetTimerManager().ListTimers();

	for (auto& Pair : BufferMap)
	{
		auto Buffer = BufferMap.Find(Pair.Key);
		if (Buffer)
		{
			bool bNewValue = GetWorld()->GetTimerManager().GetTimerElapsed(Buffer->TimerHandle) < Buffer->Time;

			if (Buffer->bActive != bNewValue && !bNewValue)
			{
				ClearBuffer(Pair.Key);
				OnBufferEnd.Broadcast(Pair.Key);
			}
		}
	}
}

