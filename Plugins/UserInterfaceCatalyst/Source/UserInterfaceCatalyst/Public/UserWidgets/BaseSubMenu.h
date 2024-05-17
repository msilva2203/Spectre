// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseSubMenu.generated.h"

class UBaseButton;

UENUM(BlueprintType)
enum class EMenuNavigationType : uint8
{
	Horizontal,
	Vertical
};

UENUM(BlueprintType)
enum class EMenuAction : uint8
{
	Accept,
	Back,
	Custom
};

USTRUCT(BlueprintType)
struct FNavContainerParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nav Container Params")
	bool bCircularNavigation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nav Container Params")
	int32 VerticalMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nav Container Params")
	int32 HorizontalMultiplier = 0;

};

USTRUCT(BlueprintType)
struct FNavigationContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation Container")
	FNavContainerParams Params;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation Container")
	TArray<UBaseButton*> Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation Container")
	int32 NavIndex;
};

/**
 * 
 */
UCLASS()
class USERINTERFACECATALYST_API UBaseSubMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeOnDirectionInput(EMenuNavigationType Type, int32 Value);
	virtual void NativeOnActionInput(EMenuAction Action, uint8 Custom = 0);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sub Menu")
	void OnDirectionInput(EMenuNavigationType Type, int32 Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sub Menu")
	void OnActionInput(EMenuAction Action, uint8 Custom = 0);

	UFUNCTION(BlueprintCallable, Category = "Sub Menu")
	int32 CreateNavContainer(const TArray<UBaseButton*>& Buttons, const FNavContainerParams& Params);

	UFUNCTION(BlueprintCallable, Category = "Sub Menu")
	int32 UpdateNavContainer(int32 ContainerIndex, int32 Value, bool bForce = false);

	UFUNCTION(BlueprintCallable, Category = "Sub Menu")
	int32 OffsetNavContainer(int32 ContainerIndex, int32 Value, EMenuNavigationType Type = EMenuNavigationType::Vertical);

	UFUNCTION(BlueprintCallable, Category = "Sub Menu")
	void CallNavContainer(int32 ContainerIndex);

private:
	UPROPERTY()
	TArray<FNavigationContainer> NavContainers;
};
