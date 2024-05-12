// © 2024 Marco Silva. All Rights Reserved.


#include "UserInterfaceSubsystem.h"
#include "UserWidgets/BaseGameDisplay.h"

void UUserInterfaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UUserInterfaceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	for (auto GameDisplay : GameDisplays)
	{
		if (GameDisplay)
			GameDisplay->RemoveFromParent();
	}
}

bool UUserInterfaceSubsystem::PushGameDisplay(UBaseGameDisplay*& OutGameDisplay, TSubclassOf<UBaseGameDisplay> GameDisplayClass,
	int32 Layer, int32 ZOrder, bool bPlayerDisplay, bool bImmediate)
{
	APlayerController* LocalPlayer = GetLocalPlayer()->PlayerController;
	OutGameDisplay = CreateWidget<UBaseGameDisplay>(LocalPlayer, GameDisplayClass);

	if (!OutGameDisplay) return false;

	if (bPlayerDisplay) OutGameDisplay->AddToPlayerScreen(ZOrder);
	else OutGameDisplay->AddToViewport(ZOrder);
	GameDisplays.Add(OutGameDisplay);

	HideGameDisplay(OutGameDisplay);
	if (bImmediate) ShowGameDisplay(OutGameDisplay, false);

	return true;
}

bool UUserInterfaceSubsystem::RemoveGameDisplay(UBaseGameDisplay* GameDisplay)
{
	if (GameDisplays.Contains(GameDisplay))
	{
		GameDisplays.Remove(GameDisplay);

		HideGameDisplay(GameDisplay);
		GameDisplay->RemoveFromParent();
		return true;
	}
	return false;
}

bool UUserInterfaceSubsystem::ShowGameDisplay(UBaseGameDisplay* GameDisplay, bool bForce)
{
	if (GameDisplay)
	{
		if (!CollapsedLayers.Contains(GameDisplay->GetLayer()) || bForce)
		{
			GameDisplay->SetVisibility(ESlateVisibility::HitTestInvisible);
			return true;
		}
	}
	return false;
}

bool UUserInterfaceSubsystem::HideGameDisplay(UBaseGameDisplay* GameDisplay)
{
	if (GameDisplay)
	{
		GameDisplay->SetVisibility(ESlateVisibility::Collapsed);
		return true;
	}
	return false;
}

void UUserInterfaceSubsystem::CollapseLayer(int32 Layer)
{
	if (!CollapsedLayers.Contains(Layer))
	{
		CollapsedLayers.Add(Layer);
		for (auto GameDisplay : GameDisplays)
		{
			if (GameDisplay->GetLayer() == Layer)
			{
				HideGameDisplay(GameDisplay);
			}
		}
	}
}

void UUserInterfaceSubsystem::DisplayLayer(int32 Layer)
{
	if (CollapsedLayers.Contains(Layer))
	{
		CollapsedLayers.Remove(Layer);
		for (auto GameDisplay : GameDisplays)
		{
			if (GameDisplay->GetLayer() == Layer)
			{
				ShowGameDisplay(GameDisplay);
			}
		}
	}
}
