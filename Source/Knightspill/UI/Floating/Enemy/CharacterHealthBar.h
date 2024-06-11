// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHealthBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class KNIGHTSPILL_API UCharacterHealthBar : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess="true"))
	UProgressBar* HealthBar;
};
