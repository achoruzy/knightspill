// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Equipment/Weapons/Weapon.h"
#include "Knightspill/Systems/Interfaces/Collectible.h"

#include "CollectibleContainer.generated.h"

UCLASS()
class KNIGHTSPILL_API ACollectibleContainer : public AActor, public ICollectible
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collider;
	UPROPERTY(EditAnywhere)
	AWeapon* Item;

	ACollectibleContainer();
	virtual void Tick(float DeltaTime) override;
	virtual void Collect_Implementation(AMainCharacter* Character) override;

protected:
	virtual void BeginPlay() override;
};
