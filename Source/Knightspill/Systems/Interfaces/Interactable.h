// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Objects/Characters/MainCharacter/MainCharacter.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KNIGHTSPILL_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Interact(AMainCharacter* Character);
	virtual void Interact_Implementation(AMainCharacter* Character) = 0;
};
