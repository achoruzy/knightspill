// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Knightspill/Objects/Items/Item.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "Destructible.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class KNIGHTSPILL_API ADestructible : public AActor, public IHittable
{
	GENERATED_BODY()

private:
	bool IsBroken = false;
	UPROPERTY(EditAnywhere, Category="Treasures", Meta = (AllowPrivateAccess = "true"))
	float TreasureChance = 0.5;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UGeometryCollectionComponent* BreakableGeometry;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CollisionCapsule;
	UPROPERTY(EditAnywhere, Category="Treasures", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AItem>> SpawnTreasures;

public:
	ADestructible();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;
};
