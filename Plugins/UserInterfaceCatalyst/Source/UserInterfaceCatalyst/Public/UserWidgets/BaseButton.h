// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "BaseButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

/**
 * 
 */
UCLASS()
class USERINTERFACECATALYST_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnClick();
	virtual void NativeOnHovered();
	virtual void NativeOnUnHovered();

	UFUNCTION(BlueprintImplementableEvent, Category = "Button")
	void OnHovered();

	UFUNCTION(BlueprintImplementableEvent, Category = "Button")
	void OnUnHovered();

	UFUNCTION(BlueprintImplementableEvent, Category = "Button")
	void OnClick();

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnButtonClicked OnClicked;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BTN;

};
