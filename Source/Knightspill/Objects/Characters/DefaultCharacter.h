// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseCharacter.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "DefaultCharacter.generated.h"

class AWeapon;

UCLASS()
class KNIGHTSPILL_API ADefaultCharacter : public ABaseCharacter, public IHittable
{
	GENERATED_BODY()

public:
protected:
	UPROPERTY()
	AWeapon* WeaponEquipped;
	//** ANIM MONTAGES */
	UPROPERTY(EditDefaultsOnly, Category="! AnimMontages")
	UAnimMontage* HitReactionMontage;
	UPROPERTY(EditDefaultsOnly, Category="! AnimMontages")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category="! AnimMontages")
	UAnimMontage* AttackMontage;
private:


public:
	ADefaultCharacter();
	UFUNCTION(BlueprintCallable)
	virtual void StartHit();
	UFUNCTION(BlueprintCallable)
	virtual void StopHit();
	UFUNCTION(BlueprintCallable)
	virtual void ResetState() {}

	/** IHittable */
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
};
