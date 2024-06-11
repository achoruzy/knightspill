// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterHealthBarComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KNIGHTSPILL_API UCharacterHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCharacterHealthBarComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
