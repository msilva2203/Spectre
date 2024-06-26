// © 2024 Marco Silva. All Rights Reserved.


#include "Components/WorldWidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWorldWidgetComponent::UWorldWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWorldWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (DisplayWidget)
	{
		DisplayWidget->RemoveFromParent();
	}
}


// Called every frame
void UWorldWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector2D ScreenPosition;
	FVector2D ViewportSize;
	FVector2D DesiredSize;
	auto WorldPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (DisplayWidget)
	{
		UGameplayStatics::ProjectWorldToScreen(WorldPlayerController, GetComponentLocation(), ScreenPosition, true);
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		DesiredSize = DisplayWidget->GetDesiredSize();

		DesiredSize.X /= 2.0f;
		DesiredSize.Y /= 2.0f;

		// Centering the widget
		ScreenPosition.X -= DesiredSize.X / 2.0f;
		ScreenPosition.Y -= DesiredSize.Y / 2.0f;

		ViewportSize -= DesiredSize;

		if (bLockOnScreen)
		{
			ScreenPosition.X = FMath::Clamp<float>(ScreenPosition.X, SafeZoneX.X, ViewportSize.X - SafeZoneX.Y);
			ScreenPosition.Y = FMath::Clamp<float>(ScreenPosition.Y, SafeZoneY.X, ViewportSize.Y - SafeZoneY.Y);
		}
		
		DisplayWidget->SetPositionInViewport(ScreenPosition, true);
	}
}

UBaseGameDisplay* UWorldWidgetComponent::PushDisplayWidget(TSubclassOf<UBaseGameDisplay> DisplayWidgetClass, APlayerController* OwningPlayer, int32 ZOrder)
{
	if (DisplayWidget)
	{
		DisplayWidget->RemoveFromParent();
	}

	DisplayWidget = CreateWidget<UBaseGameDisplay>(OwningPlayer, DisplayWidgetClass);
	if (DisplayWidget)
	{
		DisplayWidget->AddToPlayerScreen(ZOrder);
	}

	return DisplayWidget;
}

void UWorldWidgetComponent::SetSafeZone(const FVector2D& NewSafeZoneX, const FVector2D& NewSafeZoneY)
{
	SafeZoneX = NewSafeZoneX;
	SafeZoneY = NewSafeZoneY;
}

void UWorldWidgetComponent::SetLockOnScreen(bool bNewValue)
{
	bLockOnScreen = bNewValue;
}

