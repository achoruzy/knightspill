// Copyright (C) Data Verft Arkadiusz Choruzy


#include "KnightspillHUD.h"

#include "Knightspill/UI/HUD/HUDOverlay.h"

void AKnightspillHUD::BeginPlay()
{
	Super::BeginPlay();
	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UHUDOverlay>(GetWorld()->GetFirstPlayerController(), HUDOverlayClass);
		HUDOverlay->AddToViewport();
	}
}
