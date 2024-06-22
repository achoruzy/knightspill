// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KnightspillHUD.generated.h"

class UHUDOverlay;
/**
 * 
 */
UCLASS()
class KNIGHTSPILL_API AKnightspillHUD : public AHUD
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category="! HUD Defaults")
	TSubclassOf<UHUDOverlay> HUDOverlayClass;
	UPROPERTY()
	UHUDOverlay* HUDOverlay;

public:
	FORCEINLINE UHUDOverlay* GetOverlay() const { return HUDOverlay; }
	
protected:
	virtual void BeginPlay() override;
};
