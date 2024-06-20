// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Objects/Characters/DefaultCharacter.h"
#include "EnemyEnums.h"
#include "Enemy.generated.h"

struct FPathFollowingResult;
struct FAIRequestID;
class UPawnSensingComponent;
class AAIController;
class UCharacterAttributesComponent;
class UCharacterHealthBarComponent;

UCLASS()
class KNIGHTSPILL_API AEnemy : public ADefaultCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	UPawnSensingComponent* SensingComponent;
	UPROPERTY(EditDefaultsOnly)
	UCharacterHealthBarComponent* HealthBarComponent;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	EEnemyLivingStatus LifeStatus;
	UPROPERTY()
	ACharacter* Player;
	// UPROPERTY()
	// float DistanceToPlayer;
	UPROPERTY(EditAnywhere, Category="! Enemy Defaults")
	float TickInterval = 0.3f;
	UPROPERTY()
	float TickIntervalCurrent;
	UPROPERTY(EditAnywhere, Category="! Enemy Defaults")
	float ShowHealthBarRadius = 1000.f;
	UPROPERTY(EditAnywhere, Category="! Enemy Defaults")
	float AttackInterval = 3.f;
	UPROPERTY()
	float AttackIntervalCurrent;
	
	//** TARGETS */
	UPROPERTY()
	FVector TargetPosition;
	UPROPERTY()
	AActor* PatrolTarget;
	UPROPERTY()
	int PatrolTargetID = -1;
	UPROPERTY(EditAnywhere, Category="! Targets")
	float PatrolApproachRadius = 200.f;
	UPROPERTY()
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category="! Targets")
	float CombatApproachRadius = 500.f;
	UPROPERTY(EditAnywhere, Category="! Targets")
	float CombatActionRadius = 150.f;

	//** AI */
	UPROPERTY(BlueprintReadOnly, Category="! Enemy AI", meta=(AllowPrivateAccess="true"))
	EEnemyState State;
	UPROPERTY(BlueprintReadOnly, Category="! Enemy AI", meta=(AllowPrivateAccess="true"))
	AAIController* EnemyController;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="! Enemy AI", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> PatrolTargets;

	//** TIMERS */
	FTimerHandle PatrolTimer;
	
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ApproachLocation(const FVector ApproachLocation) const;
	UFUNCTION(BlueprintCallable)
	void ApproachActor(const AActor* ApproachTarget) const;
	/** DefaultCharacter */
	virtual void ResetState() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/** CALLBACKS */
	void OnApproachCompleted(FAIRequestID ID, const FPathFollowingResult& Result) const;

protected:
	virtual void BeginPlay() override;
	void Attack();
	void ToggleHealthBar(const bool Visible) const;
	void ToggleSensing(const bool DoSense) const;
	/** AI */
	void DecideBehaviour();
	bool IsInCombatRange(double DistanceToTarget) const;
	void GetIntoCombat();
	bool IsInCombat() const;
	bool IsInAttackRange(double DistanceToTarget) const;
	bool CanAttack() const;
	void ApproachCombatTarget();
	bool IsBusy() const;
	bool IsReadyToApproachPatrolTarget() const;
	void ApproachNextPatrolTarget();
	bool HasJustApproachedPatrolTarget() const;
	void WaitAtPatrolTarget();
	AActor* NextPatrolTarget();
	/** DefaultCharacter */
	virtual void Die() override;
	/** IHittable */
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;

private:
	double DistanceToPlayer() const;
	bool IsInShowHealthBarRadius() const;
	/** CALLBACKS */
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	void OnPatrolTimerFinished();
};