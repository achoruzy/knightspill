// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/UI/Floating/Enemy/CharacterHealthBarComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavMesh/NavMeshPath.h"
#include "Perception/PawnSensingComponent.h"


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

	LivingStatus = EEnemyLivingStatus::Lives;
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

	DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	CombatTarget = Player;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LivingStatus != EEnemyLivingStatus::Lives) return;
	TickIntervalCurrent += DeltaTime;
	AttackIntervalCurrent += DeltaTime;

	if (TickIntervalCurrent > TickInterval)
	{
		TickIntervalCurrent = 0.f;
		DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
		SetShowHealthBar();

		if (DistanceToPlayer < CombatApproachRadius)
		{
			State = EEnemyState::Chase;
		}
		if (State == EEnemyState::Wait) return;

		if (State == EEnemyState::Chase)
		{
			if (DistanceToPlayer < CombatActionRadius)
			{
				if(AttackIntervalCurrent > AttackInterval)
				{
					AttackIntervalCurrent = 0.f;
					Attack();
				}
			}
			else if (DistanceToPlayer >= CombatApproachRadius)
			{
				State = EEnemyState::Idle;
			}
			else
			{
				PatrolPosition = CombatTarget->GetActorLocation();
				ApproachLocation(PatrolPosition);
			}
			return;
		}

		if (State == EEnemyState::Idle && PatrolTargets.Num() > 0)
		{
			State = EEnemyState::Patrol;
			PatrolTarget = NextPatrolTarget();
			PatrolPosition = PatrolTarget->GetActorLocation();
			ApproachLocation(PatrolPosition);
			return;
		}

		if (State == EEnemyState::Patrol && FVector::Dist(GetActorLocation(), PatrolPosition) < PatrolApproachRadius)
		{
			State = EEnemyState::Wait;
			GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::OnPatrolTimerFinished, 3.f);
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnPatrolTimerFinished()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	State = EEnemyState::Idle;
}

void AEnemy::Attack()
{
	State = EEnemyState::Wait;
	PlayAnimationMontage(AttackMontage, FEnemyAttackMontageTitles::LightAttack);
}

void AEnemy::SetShowHealthBar()
{
	if (DistanceToPlayer < ShowHealthBarRadius) HealthBarComponent->SetVisibility(true);
	else HealthBarComponent->SetVisibility(false);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CharacterAttributes->ReceiveDamage(DamageAmount);
	HealthBarComponent->SetHealthPercent(CharacterAttributes->GetHealthPercent());
	
	if (!CharacterAttributes->IsAlive() && LivingStatus == EEnemyLivingStatus::Lives)
	{
		Die();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemy::ApproachActor(const AActor* ApproachTarget) const
{
	if (ApproachTarget)
	{
		ApproachLocation(ApproachTarget->GetActorLocation());
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

		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		// for (auto& Point : PathPoints)
		// {
		// 	const FVector& Location = Point.Location;
		// 	DrawDebugSphere(GetWorld(), Location, 50.f, 12, FColor::Orange, false, 10);
		// }
	}
}


void AEnemy::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal)
{
	if (!CharacterAttributes->IsAlive()) return;
	
	const FVector Forward = GetActorForwardVector();
	const FVector ToHitPos = (DamagePosition - GetActorLocation()).GetSafeNormal();
	const FVector FlattenHitPos(ToHitPos.X, ToHitPos.Y, Forward.Z);
	const double CosTheta = FVector::DotProduct(Forward, FlattenHitPos);
	const double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

	const FVector Sign = FVector::CrossProduct(Forward, FlattenHitPos);
	
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

void AEnemy::Die()
{
	switch (int Choice = FMath::RandRange(0, 1))
	{
	case 0:
		LivingStatus = EEnemyLivingStatus::DiedForward;
		PlayAnimationMontage(DeathMontage, TEXT("DieForward"));
		break;
	case 1:
		LivingStatus = EEnemyLivingStatus::DiedBack;
		PlayAnimationMontage(DeathMontage, TEXT("DieBack"));
		break;
	default:
		break;
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBarComponent->SetVisibility(false);
	SensingComponent->SetActive(false);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(false);
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

void AEnemy::OnPawnSeen(APawn* Pawn)
{
	if (Pawn->ActorHasTag(FName("MainCharacter")))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s seen!"), *Pawn->GetName());
	}
}

void AEnemy::OnApproachCompleted(FAIRequestID ID, const FPathFollowingResult& Result) const
{
	// UE_LOG(LogTemp, Warning, TEXT("Approaching completed"));
}
