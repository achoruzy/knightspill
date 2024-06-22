// Copyright (C) Data Verft Arkadiusz Choruzy


#include "HUDAttributes.h"

#include <string>

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDAttributes::SetHealthPercent(const float NewHealth) const
{
	HealthProgressBar->SetPercent(NewHealth);
}

void UHUDAttributes::SetStaminaPercent(const float NewStamina) const
{
	StaminaProgressBar->SetPercent(NewStamina);
}

void UHUDAttributes::SetSoulsValue(const int NewSouls) const
{
	SoulsText->SetText(FText::FromString(FString::FromInt(NewSouls)));
}

void UHUDAttributes::SetGoldValue(const int NewGold) const
{
	GoldText->SetText(FText::FromString(FString::FromInt(NewGold)));
}
