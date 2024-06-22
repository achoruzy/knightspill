// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDAttributes.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class KNIGHTSPILL_API UHUDAttributes : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;
	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaProgressBar;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SoulsText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldText;
	UPROPERTY(meta=(BindWidget))
	UImage* OmegaImage;

public:
	void SetHealthPercent(const float NewHealth) const;
	void SetStaminaPercent(const float NewStamina) const;
	void SetSoulsValue(const int NewSouls) const;
	void SetGoldValue(const int NewGold) const;
};
