// Copyright (C) Data Verft Arkadiusz Choruzy


#include "HUDOverlay.h"

#include "HUDAttributes.h"

void UHUDOverlay::SetHealthPercent(const float Percent) const
{
	AttributesWidget->SetHealthPercent(Percent);
}

void UHUDOverlay::SetStaminaPercent(const float NewStamina) const
{
	AttributesWidget->SetStaminaPercent(NewStamina);
}

void UHUDOverlay::SetSoulsValue(const int NewSouls) const
{
	AttributesWidget->SetSoulsValue(NewSouls);
}

void UHUDOverlay::SetGoldValue(const int NewGold) const
{
	AttributesWidget->SetGoldValue(NewGold);
}
