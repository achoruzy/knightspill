// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseCharacter.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "DefaultCharacter.generated.h"

class UCharacterAttributesComponent;
class AWeapon;

UCLASS()
class KNIGHTSPILL_API ADefaultCharacter : public ABaseCharacter, public IHittable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	UCharacterAttributesComponent* CharacterAttributes;
	UPROPERTY(VisibleInstanceOnly, Category="! Weapon")
	AWeapon* Weapon;
	//** ANIM MONTAGES */
	UPROPERTY(EditDefaultsOnly, Category="! AnimMontages")
	UAnimMontage* HitReactionMontage;
	UPROPERTY(EditDefaultsOnly, Category="! AnimMontages")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category="! AnimMontages")
	UAnimMontage* AttackMontage;

public:
	ADefaultCharacter();
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable)
	virtual void StartHit();
	UFUNCTION(BlueprintCallable)
	virtual void StopHit();
	UFUNCTION(BlueprintCallable)
	virtual void ResetState() {}
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/** IHittable */
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Die();
private:
};
