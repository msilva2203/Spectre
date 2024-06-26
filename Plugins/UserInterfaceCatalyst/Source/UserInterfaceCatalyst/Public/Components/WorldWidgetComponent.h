// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UserWidgets/BaseGameDisplay.h"
#include "WorldWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USERINTERFACECATALYST_API UWorldWidgetComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "World Widget")
	UBaseGameDisplay* PushDisplayWidget(TSubclassOf<UBaseGameDisplay> DisplayWidgetClass, APlayerController* OwningPlayer, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "World Widget")
	void SetSafeZone(const FVector2D& NewSafeZoneX, const FVector2D& NewSafeZoneY);

	UFUNCTION(BlueprintCallable, Category = "World Widget")
	void SetLockOnScreen(bool bNewValue);

	UPROPERTY(EditAnywhere, Category = "World Widget")
	FVector2D SafeZoneX;

	UPROPERTY(EditAnywhere, Category = "World Widget")
	FVector2D SafeZoneY;

	UPROPERTY(EditAnywhere, Category = "World Widget")
	bool bLockOnScreen;

private:
	UPROPERTY()
	UBaseGameDisplay* DisplayWidget;

};
