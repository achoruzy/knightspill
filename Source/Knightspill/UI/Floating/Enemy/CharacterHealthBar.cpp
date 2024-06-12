// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CharacterHealthBar.h"

#include "Components/ProgressBar.h"


void UCharacterHealthBar::SetHealthPercent(const float Percent) const
{
	HealthBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
}
