// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseActor.h"

#include "Item.generated.h"


/// @brief Item class is a base class for all collectible items of any kind
UCLASS()
class KNIGHTSPILL_API AItem : public ABaseActor
{
	GENERATED_BODY()

public:
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
