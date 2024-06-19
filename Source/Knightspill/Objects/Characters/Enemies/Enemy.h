// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Game/EngineClassExtensions/BaseCharacter.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/Objects/Characters/DefaultCharacter.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"
#include "Enemy.generated.h"

struct FPathFollowingResult;
struct FAIRequestID;
class UPawnSensingComponent;
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
	Wait,
	Patrol,
	Chase,
	Attack
};

struct FEnemyAttackMontageTitles
{
	inline static FName LightAttack = "LightAttack";
};

UCLASS()
class KNIGHTSPILL_API AEnemy : public ADefaultCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* SensingComponent;
	UPROPERTY(EditDefaultsOnly)
	UCharacterAttributesComponent* CharacterAttributes;
	UPROPERTY(EditDefaultsOnly)
	UCharacterHealthBarComponent* HealthBarComponent;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	EEnemyLivingStatus LivingStatus;
	UPROPERTY()
	ACharacter* Player;
	UPROPERTY()
	float DistanceToPlayer;
	UPROPERTY(EditAnywhere)
	float TickInterval = 0.3f;
	UPROPERTY()
	float TickIntervalCurrent;
	UPROPERTY(EditAnywhere)
	float ShowHealthBarRadius = 1000.f;
	UPROPERTY(EditAnywhere)
	float AttackInterval = 3.f;
	UPROPERTY()
	float AttackIntervalCurrent;

	//** TIMERS AND RELATED */
	FTimerHandle PatrolTimer;
	void OnPatrolTimerFinished();

	//** TARGETS */
	UPROPERTY()
	AActor* PatrolTarget;
	UPROPERTY(EditAnywhere, Category="! Targets")
	float PatrolApproachRadius = 200.f;
	UPROPERTY()
	int PatrolTargetID = -1;
	UPROPERTY()
	
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category="! Targets")
	float CombatApproachRadius = 500.f;
	UPROPERTY(EditAnywhere, Category="! Targets")
	float CombatActionRadius = 150.f;
	UPROPERTY()
	FVector PatrolPosition;

	//** AI */
	UPROPERTY(BlueprintReadOnly, Category="! Enemy AI", meta=(AllowPrivateAccess="true"))
	EEnemyState State;
	UPROPERTY(BlueprintReadOnly, Category="! Enemy AI", meta=(AllowPrivateAccess="true"))
	AAIController* EnemyController;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="! Enemy AI", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> PatrolTargets;
	
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return CharacterAttributes->IsAlive(); }
	UFUNCTION(BlueprintCallable)
	void ApproachActor(const AActor* ApproachTarget) const;
	UFUNCTION(BlueprintCallable)
	void ApproachLocation(const FVector ApproachLocation) const;
	void OnApproachCompleted(FAIRequestID ID, const FPathFollowingResult& Result) const;
	virtual void ResetState() override { State = EEnemyState::Idle; }

	
protected:
	virtual void BeginPlay() override;
	void SetShowHealthBar();
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;
	void Die();
	AActor* NextPatrolTarget();
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	void Attack();
};
