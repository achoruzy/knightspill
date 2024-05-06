// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Collectible.generated.h"

UINTERFACE()
class UCollectible : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KNIGHTSPILL_API ICollectible
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void Collect(AMainCharacter* Character);
	virtual void Collect_Implementation(AMainCharacter* Character) = 0;
};
