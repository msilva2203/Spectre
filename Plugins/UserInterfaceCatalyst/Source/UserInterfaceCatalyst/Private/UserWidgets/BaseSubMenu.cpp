// © 2024 Marco Silva. All Rights Reserved.


#include "UserWidgets/BaseSubMenu.h"
#include "UserWidgets/BaseButton.h"

void UBaseSubMenu::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBaseSubMenu::NativeOnDirectionInput(EMenuNavigationType Type, int32 Value)
{
	OnDirectionInput(Type, Value);

}

void UBaseSubMenu::NativeOnActionInput(EMenuAction Action, uint8 Custom)
{
	OnActionInput(Action, Custom);

	if (Action == EMenuAction::Back)
	{
		OnBack.Broadcast();
	}
}

void UBaseSubMenu::NativeOnMenuFocus()
{
	SetVisibility(FocusedVisibility);
	OnMenuFocus();

}

void UBaseSubMenu::NativeOnMenuUnfocus()
{
	SetVisibility(UnfocusedVisibility);
	OnMenuUnfocus();

}

int32 UBaseSubMenu::CreateNavContainer(const TArray<UBaseButton*>& Buttons, const FNavContainerParams& Params)
{
	FNavigationContainer NavContainer;

	NavContainer.Params = Params;
	NavContainer.Buttons = Buttons;
	NavContainer.NavIndex = 0;

	int32 Index = NavContainers.Add(NavContainer);

	for (auto& Btn : NavContainer.Buttons)
	{
		Btn->NativeOnUnHovered();
		Btn->NavContainerIndex = Index;
		Btn->OnHoverHelper.AddDynamic(this, &UBaseSubMenu::OnContainerButtonHovered);
	}

	UpdateNavContainer(Index, 0, true);
	return Index;
}

int32 UBaseSubMenu::UpdateNavContainer(int32 ContainerIndex, int32 Value, bool bForce)
{
	FNavigationContainer* NavContainer = &NavContainers[ContainerIndex];

	if (NavContainer)
	{
		int32 Num = NavContainer->Buttons.Num();
		if (Value < 0 || Value > Num - 1)
		{
			if (NavContainer->Params.bCircularNavigation)
			{
				if (Value < 0)
				{
					Value = Num - 1;
				}
				else
				{
					Value = 0;
				}
			}
			else
			{
				Value = FMath::Clamp<int32>(Value, 0, Num - 1);
			}
		}
		if (Value != NavContainer->NavIndex || bForce)
		{
			NavContainer->Buttons[NavContainer->NavIndex]->NativeOnUnHovered();
			NavContainer->Buttons[Value]->NativeOnHovered();
			NavContainer->NavIndex = Value;
		}
	}
	return Value;
}

int32 UBaseSubMenu::OffsetNavContainer(int32 ContainerIndex, int32 Value, EMenuNavigationType Type)
{
	int32 NewIndex = -1;
	FNavigationContainer* NavContainer = &NavContainers[ContainerIndex];

	if (NavContainer)
	{
		int32 Multiplier = NavContainer->Params.VerticalMultiplier;
		if (Type != EMenuNavigationType::Vertical)
		{
			Multiplier = NavContainer->Params.HorizontalMultiplier;
		}
		Value *= Multiplier;

		NewIndex = NavContainer->NavIndex + Value;
		NewIndex = UpdateNavContainer(ContainerIndex, NewIndex);
	}
	return NewIndex;
}

void UBaseSubMenu::CallNavContainer(int32 ContainerIndex)
{
	FNavigationContainer* NavContainer = &NavContainers[ContainerIndex];
	if (NavContainer)
	{
		NavContainer->Buttons[NavContainer->NavIndex]->NativeOnClick();
	}
}

void UBaseSubMenu::OnContainerButtonHovered(int32 NavIndex, int32 ContainerIndex)
{
	UpdateNavContainer(ContainerIndex, NavIndex, false);
	
}
