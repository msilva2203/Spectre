// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeMapComponent.generated.h"

class UIntegerAttributeComponent;
class UScalarAttributeComponent;
class UByteAttributeComponent;
class UBoolAttributeComponent;
class UActorAttributeComponent;

/**
 * This component is used by the catalyst statics library to find attribute components more efficiently.
 * Can be ignored by the game designers
 */
UCLASS( ClassGroup=(Catalyst), meta=(BlueprintSpawnableComponent), Category = "Multiplayer Catalyst" )
class MULTIPLAYERCATALYST_API UAttributeMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeMapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attribute Map")
	UIntegerAttributeComponent* FindIntegerAttribute(FName ID);

	UFUNCTION(BlueprintCallable, Category = "Attribute Map")
	UScalarAttributeComponent* FindScalarAttribute(FName ID);

	UFUNCTION(BlueprintCallable, Category = "Attribute Map")
	UByteAttributeComponent* FindByteAttribute(FName ID);

	UFUNCTION(BlueprintCallable, Category = "Attribute Map")
	UBoolAttributeComponent* FindBoolAttribute(FName ID);

	UFUNCTION(BlueprintCallable, Category = "Attribute Map")
	UActorAttributeComponent* FindActorAttribute(FName ID);

	bool AddIntegerAttribute(UIntegerAttributeComponent* NewComponent);
	bool AddScalarAttribute(UScalarAttributeComponent* NewComponent);
	bool AddByteAttribute(UByteAttributeComponent* NewComponent);
	bool AddBoolAttribute(UBoolAttributeComponent* NewComponent);
	bool AddActorAttribute(UActorAttributeComponent* NewComponent);

private:
	UPROPERTY()
	TMap<FName, UActorComponent*> AttributeMap;
};
