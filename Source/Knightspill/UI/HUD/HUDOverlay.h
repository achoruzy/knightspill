// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDOverlay.generated.h"

class UHUDAttributes;
/**
 * 
 */
UCLASS()
class KNIGHTSPILL_API UHUDOverlay : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UHUDAttributes* AttributesWidget;

public:
	void SetHealthPercent(const float Percent) const;
	void SetStaminaPercent(const float NewStamina) const;
	void SetSoulsValue(const int NewSouls) const;
	void SetGoldValue(const int NewGold) const;
};
