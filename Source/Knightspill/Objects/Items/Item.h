// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseActor.h"
#include "Knightspill/Systems/Interfaces/Collectible.h"

#include "Item.generated.h"


/// @brief Item class is a base class for all collectible items of any kind
UCLASS()
class KNIGHTSPILL_API AItem : public ABaseActor, public ICollectible
{
	GENERATED_BODY()

public:
	AItem();
	virtual void Tick(float DeltaTime) override;
	virtual void Collect_Implementation(AMainCharacter* Character) override {};

protected:
	virtual void BeginPlay() override;
};
