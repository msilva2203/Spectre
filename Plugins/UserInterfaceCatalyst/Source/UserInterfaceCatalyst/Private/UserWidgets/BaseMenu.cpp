// © 2024 Marco Silva. All Rights Reserved.


#include "UserWidgets/BaseMenu.h"
#include "UserWidgets/BaseSubMenu.h"
#include "Kismet/KismetInputLibrary.h"

void UBaseMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	bIsFocusable = true;
}

void UBaseMenu::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBaseMenu::NativeOnShow()
{
	OnShow();
	OnMenuShown.Broadcast();

}

void UBaseMenu::NativeOnHide()
{
	OnHide();
	OnMenuHidden.Broadcast();
	
}

FReply UBaseMenu::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	EUINavigation NavigationDirection;
	EUINavigationAction NavigationAction;

	// 1. Get key
	//FKey Key = InKeyEvent.GetKey();

	// 2. Get navigation action
	NavigationAction = UKismetInputLibrary::Key_GetNavigationActionFromKey(InKeyEvent);

	if (NavigationAction != EUINavigationAction::Invalid)
	{
		if (!InKeyEvent.IsRepeat())
			HandleActionInput(NavigationAction);
		return FReply::Handled();
	}

	// 3. Get navigation direction
	NavigationDirection = UKismetInputLibrary::Key_GetNavigationDirectionFromKey(InKeyEvent);

	if (NavigationDirection != EUINavigation::Invalid)
	{
		HandleDirectionInput(NavigationDirection);
		return FReply::Handled();
	}

	return FReply::Handled();
}

FReply UBaseMenu::NativeOnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogEvent)
{
	EUINavigation NavigationDirection;

	NavigationDirection = UKismetInputLibrary::Key_GetNavigationDirectionFromAnalog(InAnalogEvent);
	
	if (NavigationDirection != EUINavigation::Invalid)
	{
		HandleDirectionInput(NavigationDirection);
		return FReply::Handled();
	}

	return FReply::Handled();
}

void UBaseMenu::FocusSubMenu(UBaseSubMenu* SubMenu)
{
	if (!SubMenu) return;

	if (FocusedSubMenu)
	{
		FocusedSubMenu->NativeOnMenuUnfocus();
	}

	FocusedSubMenu = SubMenu;
	FocusedSubMenu->NativeOnMenuFocus();
}

void UBaseMenu::HandleActionInput(EUINavigationAction NavigationAction, uint8 Custom)
{
	if (!FocusedSubMenu) return;

	switch (NavigationAction)
	{
	case EUINavigationAction::Accept:
		FocusedSubMenu->NativeOnActionInput(EMenuAction::Accept);
		break;
	case EUINavigationAction::Back:
		FocusedSubMenu->NativeOnActionInput(EMenuAction::Back);
		break;
	case EUINavigationAction::Invalid:
		FocusedSubMenu->NativeOnActionInput(EMenuAction::Custom, Custom);
		break;
	default:
		break;
	}
}

void UBaseMenu::HandleDirectionInput(EUINavigation NavigationDirection)
{
	if (!FocusedSubMenu) return;

	switch (NavigationDirection)
	{
	case EUINavigation::Left:
		FocusedSubMenu->NativeOnDirectionInput(EMenuNavigationType::Horizontal, -1);
		break;
	case EUINavigation::Right:
		FocusedSubMenu->NativeOnDirectionInput(EMenuNavigationType::Horizontal, 1);
		break;
	case EUINavigation::Up:
		FocusedSubMenu->NativeOnDirectionInput(EMenuNavigationType::Vertical, -1);
		break;
	case EUINavigation::Down:
		FocusedSubMenu->NativeOnDirectionInput(EMenuNavigationType::Vertical, 1);
		break;
	case EUINavigation::Next:
	case EUINavigation::Previous:
	case EUINavigation::Num:
	case EUINavigation::Invalid:
	default:
		break;
	}
}
