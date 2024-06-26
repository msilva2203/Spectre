// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEquipment.generated.h"

UCLASS()
class SPECTRE_API ABaseEquipment : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseEquipment();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipmentAction(int32 Action, bool bValue);

	//UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	//void OnEquipmentAction(int32 Action, bool bValue);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	//UIntegerAttributeComponent* AmountAttribute;

protected:
	virtual void BeginPlay() override;

	//UFUNCTION()
	//virtual void NativeOnEquipmentAction(int32 Action, bool bValue);

public:	
	virtual void Tick(float DeltaTime) override;

private:
	//UFUNCTION(Server, Reliable)
	//void Server_EquipmentAction(int32 Action, bool bValue);
	//void Server_EquipmentAction_Implementation(int32 Action, bool bValue);
	//bool Server_EquipmentAction_Validation(int32 Action, bool bValue);
	//
	//UFUNCTION(NetMulticast, Reliable)
	//void NetAll_EquipmentAction(int32 Action, bool bValue);
	//void NetAll_EquipmentAction_Implementation(int32 Action, bool bValue);
};
