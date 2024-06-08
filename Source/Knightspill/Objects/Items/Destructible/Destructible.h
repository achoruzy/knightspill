// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "Destructible.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class KNIGHTSPILL_API ADestructible : public AActor, public IHittable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UGeometryCollectionComponent* BreakableGeometry;

public:
	ADestructible();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;
};
