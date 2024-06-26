// © 2024 Marco Silva. All Rights Reserved.


#include "UserWidgets/BaseButton.h"

void UBaseButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BTN)
	{
		BTN->OnClicked.AddDynamic(this, &UBaseButton::NativeOnClick);
		BTN->OnHovered.AddDynamic(this, &UBaseButton::NativeOnMouseHovered);
		//BTN->OnUnhovered.AddDynamic(this, &UBaseButton::NativeOnUnHovered);
	}
}

void UBaseButton::NativeOnClick()
{
	OnClick();
	OnClicked.Broadcast();

}

void UBaseButton::NativeOnHovered()
{
	OnHovered();
	OnButtonHovered.Broadcast();

}

void UBaseButton::NativeOnMouseHovered()
{
	OnHoverHelper.Broadcast(NavIndex, NavContainerIndex);
}

void UBaseButton::NativeOnUnHovered()
{
	OnUnHovered();
	OnButtonUnhovered.Broadcast();

}
