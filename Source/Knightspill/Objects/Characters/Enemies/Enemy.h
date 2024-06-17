﻿// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseCharacter.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "Enemy.generated.h"

class AAIController;
class UCharacterAttributesComponent;
class UCharacterHealthBarComponent;

UENUM(BlueprintType)
enum class EEnemyLivingStatus : uint8
{
	Lives,
	DiedForward,
	DiedBack
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Patrol,
	Attack
};

UCLASS()
class KNIGHTSPILL_API AEnemy : public ABaseCharacter, public IHittable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	UCharacterAttributesComponent* CharacterAttributes;
	UPROPERTY(EditDefaultsOnly)
	UCharacterHealthBarComponent* HealthBarComponent;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	EEnemyLivingStatus LivingStatus;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	EEnemyState State;
	UPROPERTY()
	ACharacter* Player;
	UPROPERTY()
	float DistanceToPlayer;
	UPROPERTY(EditAnywhere)
	float TickTime = 0.1f;
	UPROPERTY()
	float TickTimeCurrent;
	UPROPERTY(EditAnywhere)
	float ShowHealthBarRadius = 1000.f;

	//** TARGETS */
	UPROPERTY()
	AActor* ApproachingTarget;
	UPROPERTY(EditAnywhere)
	float ApproachingRadius = 200.f;
	UPROPERTY()
	int ApproachingTargetID = -1;
	UPROPERTY()
	AActor* AttackingTarget;
	UPROPERTY(EditAnywhere)
	float AttackingRadius = 400.f;
	
	//** ANIM MONTAGES */
	UPROPERTY(EditDefaultsOnly, Category="AnimMontages")
	UAnimMontage* HitReactionMontage;
	UPROPERTY(EditDefaultsOnly, Category="AnimMontages")
	UAnimMontage* DeathMontage;

	//** AI */
	UPROPERTY(BlueprintReadOnly, Category="Enemy AI", meta=(AllowPrivateAccess="true"))
	AAIController* EnemyController;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Enemy AI", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> PatrolTargets;
	
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return CharacterAttributes->IsAlive(); }
	UFUNCTION(BlueprintCallable)
	void ApproachTarget(const AActor* ApproachTarget) const;
	void OnApproachCompleted();
	
protected:
	virtual void BeginPlay() override;
	void SetShowHealthBar();
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;
	void Die();
	AActor* NextApproachingTarget();
};
