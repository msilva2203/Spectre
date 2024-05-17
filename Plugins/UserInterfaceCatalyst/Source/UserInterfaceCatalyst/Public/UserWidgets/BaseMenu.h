// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenu.generated.h"

class UBaseSubMenu;

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

	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void FocusSubMenu(UBaseSubMenu* SubMenu);

private:
	UPROPERTY()
	UBaseSubMenu* FocusedSubMenu;

	void HandleActionInput(EUINavigationAction NavigationAction, uint8 Custom = 0);
	void HandleDirectionInput(EUINavigation NavigationDirection);
};
