// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Objects/Characters/MainCharacter/MainCharacter.h"
#include "UObject/Interface.h"
#include "Attachable.generated.h"

UINTERFACE()
class UAttachable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KNIGHTSPILL_API IAttachable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void Attach(AMainCharacter* Character);
	virtual void Attach_Implementation(AMainCharacter* Character) = 0;
};
