// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenu.generated.h"

class UBaseSubMenu;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVisibilityUpdate);

/**
 * 
 */
UCLASS()
class USERINTERFACECATALYST_API UBaseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual void NativeOnShow();
	virtual void NativeOnHide();

	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void FocusSubMenu(UBaseSubMenu* SubMenu);

	UFUNCTION(BlueprintImplementableEvent, Category = "Menu")
	void OnShow();

	UFUNCTION(BlueprintImplementableEvent, Category = "Menu")
	void OnHide();

	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnVisibilityUpdate OnMenuShown;

	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnVisibilityUpdate OnMenuHidden;

private:
	UPROPERTY()
	UBaseSubMenu* FocusedSubMenu;

	void HandleActionInput(EUINavigationAction NavigationAction, uint8 Custom = 0);
	void HandleDirectionInput(EUINavigation NavigationDirection);
};
