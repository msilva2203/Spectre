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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
