// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterHealthBarComponent.generated.h"


class UCharacterHealthBar;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KNIGHTSPILL_API UCharacterHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	UCharacterHealthBar* HealthBar;
	
public:
	UCharacterHealthBarComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void SetHealthPercent(const float Percent);
	
protected:
	virtual void BeginPlay() override;
};
