// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseCharacter.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "Enemy.generated.h"

UCLASS()
class KNIGHTSPILL_API AEnemy : public ABaseCharacter, public IHittable
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;

private:
	//** ANIM MONTAGES */
	UPROPERTY(EditDefaultsOnly, Category="AnimMontages")
	UAnimMontage* HitReactionMontage;
};
