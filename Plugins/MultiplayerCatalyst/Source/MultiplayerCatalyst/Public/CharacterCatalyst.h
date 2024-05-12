// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CatalystInterface.h"
#include "CharacterCatalyst.generated.h"

class UAttributeMapComponent;

UCLASS()
class MULTIPLAYERCATALYST_API ACharacterCatalyst : public ACharacter, public ICatalystInterface
{
	GENERATED_BODY()

public:
	ACharacterCatalyst();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Catalyst")
	void OnSetupCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void SetupCharacter();
	virtual void OnRep_PlayerState() override;

	virtual uint8 GetTeam() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	UAttributeMapComponent* AttributeMap;

private:
	
};
