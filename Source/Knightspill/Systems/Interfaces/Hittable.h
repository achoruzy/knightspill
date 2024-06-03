// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

UINTERFACE()
class UHittable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KNIGHTSPILL_API IHittable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal);
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) = 0;
};
