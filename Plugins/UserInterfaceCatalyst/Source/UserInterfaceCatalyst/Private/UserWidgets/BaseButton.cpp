// © 2024 Marco Silva. All Rights Reserved.


#include "UserWidgets/BaseButton.h"

void UBaseButton::NativeOnInitialized()
{
	if (BTN)
	{
		BTN->OnClicked.AddDynamic(this, &UBaseButton::NativeOnClick);
		BTN->OnHovered.AddDynamic(this, &UBaseButton::NativeOnHovered);
		BTN->OnUnhovered.AddDynamic(this, &UBaseButton::NativeOnUnHovered);
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

}

void UBaseButton::NativeOnUnHovered()
{
	OnUnHovered();

}
