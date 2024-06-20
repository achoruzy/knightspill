// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/UI/Floating/Enemy/CharacterHealthBarComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));

	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributesComponent>(TEXT("ArrtibutesComponent"));

	HealthBarComponent = CreateDefaultSubobject<UCharacterHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(RootComponent);

	LifeStatus = EEnemyLivingStatus::Lives;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	EnemyController = Cast<AAIController>(GetController());
	// EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemy::OnApproachCompleted);

	SensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
	
	HealthBarComponent->SetHealthPercent(1.f);
	HealthBarComponent->SetVisibility(false);

	CombatTarget = Player;
	ResetState();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LifeStatus != EEnemyLivingStatus::Lives) return;
	TickIntervalCurrent += DeltaTime;
	AttackIntervalCurrent += DeltaTime;

	if (TickIntervalCurrent > TickInterval)
	{
		TickIntervalCurrent = 0.f;
		ToggleHealthBar(IsInShowHealthBarRadius());

		DecideBehaviour();
	}
}

void AEnemy::ApproachLocation(const FVector ApproachLocation) const
{
	if (EnemyController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(ApproachLocation);
		MoveRequest.SetAcceptanceRadius(100.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);

		/** DEBUG */
		// TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		// for (auto& Point : PathPoints)
		// {
		// 	const FVector& Location = Point.Location;
		// 	DrawDebugSphere(GetWorld(), Location, 50.f, 12, FColor::Orange, false, 10);
		// }
	}
}

void AEnemy::ApproachActor(const AActor* ApproachTarget) const
{
	if (ApproachTarget)
	{
		ApproachLocation(ApproachTarget->GetActorLocation());
	}
}

void AEnemy::ResetState()
{
	State = EEnemyState::Idle;
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damaged = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthBarComponent->SetHealthPercent(CharacterAttributes->GetHealthPercent());
	return Damaged;
}

void AEnemy::OnApproachCompleted(FAIRequestID ID, const FPathFollowingResult& Result) const
{
	// UE_LOG(LogTemp, Warning, TEXT("Approaching completed"));
}

void AEnemy::Attack()
{
	State = EEnemyState::Wait;
	PlayAnimationMontage(AttackMontage, FEnemyAttackMontageTitles::LightAttack);
}

void AEnemy::DecideBehaviour()
{
	const double DistToPlayer = DistanceToPlayer();
	if (IsInCombatRange(DistToPlayer) && !IsInCombat())
	{
		GetIntoCombat();
	}
	if (IsBusy()) return;

	if (IsInCombat())
	{
		if (IsInAttackRange(DistToPlayer) && CanAttack())
		{
			AttackIntervalCurrent = 0.f;
			Attack();
			return;
		}
		
		if (IsInCombatRange(DistToPlayer))
		{
			ApproachCombatTarget();
		}
		else
		{
			ResetState();
		}
		return;
	}

	if (IsReadyToApproachPatrolTarget())
	{
		ApproachNextPatrolTarget();
		return;
	}

	if (HasJustApproachedPatrolTarget())
	{
		WaitAtPatrolTarget();
	}
}

void AEnemy::ToggleHealthBar(const bool Visible) const
{
	HealthBarComponent->SetVisibility(Visible);
}

void AEnemy::ToggleSensing(const bool DoSense) const
{
	SensingComponent->SetActive(DoSense);
}

bool AEnemy::IsInCombatRange(const double DistanceToTarget) const
{
	return DistanceToTarget < CombatApproachRadius;
}

void AEnemy::GetIntoCombat()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	State = EEnemyState::Chase;
}

bool AEnemy::IsInCombat() const
{
	return State == EEnemyState::Chase;
}

bool AEnemy::IsInAttackRange(const double DistanceToTarget) const
{
	return DistanceToTarget < CombatActionRadius;
}

bool AEnemy::CanAttack() const
{
	return AttackIntervalCurrent > AttackInterval;
}

void AEnemy::ApproachCombatTarget()
{
	TargetPosition = CombatTarget->GetActorLocation();
	ApproachLocation(TargetPosition);
}

bool AEnemy::IsBusy() const
{
	return State == EEnemyState::Wait;
}

bool AEnemy::IsReadyToApproachPatrolTarget() const
{
	return State == EEnemyState::Idle && PatrolTargets.Num() > 0;
}

void AEnemy::ApproachNextPatrolTarget()
{
	State = EEnemyState::Patrol;
	PatrolTarget = NextPatrolTarget();
	TargetPosition = PatrolTarget->GetActorLocation();
	ApproachLocation(TargetPosition);
}

bool AEnemy::HasJustApproachedPatrolTarget() const
{
	return State == EEnemyState::Patrol && FVector::Dist(GetActorLocation(), TargetPosition) < PatrolApproachRadius;
}

void AEnemy::WaitAtPatrolTarget()
{
	State = EEnemyState::Wait;
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::OnPatrolTimerFinished, 3.f);
}

AActor* AEnemy::NextPatrolTarget()
{
	if (PatrolTargets.Num() > 0)
	{
		PatrolTargetID++;
		if (PatrolTargetID == PatrolTargets.Num()) PatrolTargetID = 0;
		return PatrolTargets[PatrolTargetID];
	}
	return nullptr;
}

void AEnemy::Die()
{
	Super::Die();
	
	switch (FMath::RandRange(0, 1))
	{
	case 0:
		LifeStatus = EEnemyLivingStatus::DiedForward;
		PlayAnimationMontage(DeathMontage, TEXT("DieForward"));
		break;
	case 1:
		LifeStatus = EEnemyLivingStatus::DiedBack;
		PlayAnimationMontage(DeathMontage, TEXT("DieBack"));
		break;
	default:
		break;
	}
	ToggleHealthBar(false);
	ToggleSensing(false);
}

void AEnemy::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal)
{
	if (!IsAlive()) return;
	
	const FVector Forward = GetActorForwardVector();
	const FVector ToHitPos = (DamagePosition - GetActorLocation()).GetSafeNormal();
	const FVector FlattenHitPos(ToHitPos.X, ToHitPos.Y, Forward.Z);
	const double CosTheta = FVector::DotProduct(Forward, FlattenHitPos);
	const double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

	if (Theta > -45.f && Theta < 45.f)
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactFront"));
	}
	else if (Theta <= -45.f && Theta >= -135.f)
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactLeft"));
	}
	else if (Theta >= 45.f && Theta <= -135.f)
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactRight"));
	}
	else
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactBack"));
	}
}

double AEnemy::DistanceToPlayer() const
{
	return FVector::Dist(GetActorLocation(), Player->GetActorLocation());
}

bool AEnemy::IsInShowHealthBarRadius() const
{
	return DistanceToPlayer() < ShowHealthBarRadius;
}

void AEnemy::OnPawnSeen(APawn* Pawn)
{
	if (Pawn->ActorHasTag(FName("EnemyHittable")))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s seen!"), *Pawn->GetName());
	}
}

void AEnemy::OnPatrolTimerFinished()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	State = EEnemyState::Idle;
}
