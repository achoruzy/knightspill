// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseCharacter.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "Enemy.generated.h"

class UCharacterAttributesComponent;
class UCharacterHealthBarComponent;

UCLASS()
class KNIGHTSPILL_API AEnemy : public ABaseCharacter, public IHittable
{
	GENERATED_BODY()

private:
	//** ANIM MONTAGES */
	UPROPERTY(EditDefaultsOnly, Category="AnimMontages")
	UAnimMontage* HitReactionMontage;
	UPROPERTY(EditDefaultsOnly, Category="AnimMontages")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly)
	UCharacterAttributesComponent* CharacterAttributes;
	UPROPERTY(EditDefaultsOnly)
	UCharacterHealthBarComponent* HealthBarComponent;
	
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return CharacterAttributes->IsAlive(); }
	
protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;
	void Die() const;
};
