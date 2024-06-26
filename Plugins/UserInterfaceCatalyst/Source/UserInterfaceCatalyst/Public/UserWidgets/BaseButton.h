// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "BaseButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonContainerHelper, int32, NavIndex, int32, NavContainerIndex);

/**
 * 
 */
UCLASS()
class USERINTERFACECATALYST_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	virtual void NativeOnClick();

	UFUNCTION()
	virtual void NativeOnHovered();

	UFUNCTION()
	virtual void NativeOnMouseHovered();

	UFUNCTION()
	virtual void NativeOnUnHovered();

	UFUNCTION(BlueprintImplementableEvent, Category = "Button")
	void OnHovered();

	UFUNCTION(BlueprintImplementableEvent, Category = "Button")
	void OnUnHovered();

	UFUNCTION(BlueprintImplementableEvent, Category = "Button")
	void OnClick();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	int32 NavIndex;

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnButtonAction OnClicked;

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnButtonAction OnButtonHovered;

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnButtonAction OnButtonUnhovered;

	UPROPERTY()
	FOnButtonContainerHelper OnHoverHelper;

	UPROPERTY()
	int32 NavContainerIndex;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BTN;

};
