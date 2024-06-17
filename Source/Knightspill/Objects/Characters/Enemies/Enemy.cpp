// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/UI/Floating/Enemy/CharacterHealthBarComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavMesh/NavMeshPath.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

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
	
	HealthBarComponent->SetHealthPercent(1.f);
	HealthBarComponent->SetVisibility(false);

	DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	ApproachingTarget = NextApproachingTarget();
	ApproachTarget(ApproachingTarget);
	State = EEnemyState::Patrol;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LivingStatus != EEnemyLivingStatus::Lives) return;
	TickTimeCurrent += DeltaTime;

	if (TickTimeCurrent > TickTime)
	{
		TickTimeCurrent = 0.f;
		DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
		SetShowHealthBar();
		
		if (DistanceToPlayer < AttackingRadius && State != EEnemyState::Attack)
		{
			AttackingTarget = Player;
			State = EEnemyState::Attack;
			ApproachTarget(AttackingTarget);
			return;
		}
		// else
		// {
		// 	State = EEnemyState::Patrol;
		// }
		
		// if (State == EEnemyState::Idle && PatrolTargets.Num() > 0)
		// {
		// 	State = EEnemyState::Patrol;
		// 	ApproachTarget(ApproachingTarget);
		// }
		
		// if (State == EEnemyState::Patrol && FVector::Dist(GetActorLocation(), ApproachingTarget->GetActorLocation()) < ApproachingRadius)
		// {
		// 	// AttackingTarget = nullptr;
		// 	ApproachingTarget = NextApproachingTarget();
		// 	ApproachTarget(ApproachingTarget);
		// }
		
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::SetShowHealthBar()
{
	if (DistanceToPlayer < ShowHealthBarRadius)
	{
		HealthBarComponent->SetVisibility(true);
	}
	else
	{
		HealthBarComponent->SetVisibility(false);
	}
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

void AEnemy::ApproachTarget(const AActor* ApproachTarget) const
{
	if (EnemyController && ApproachTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(ApproachTarget);
		MoveRequest.SetAcceptanceRadius(100.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		// EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemy::OnApproachCompleted);

		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 50.f, 12, FColor::Orange, false, 10);
		}
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
	// GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	// GetMesh()->SetGenerateOverlapEvents(false);
	// GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

AActor* AEnemy::NextApproachingTarget()
{
	if (PatrolTargets.Num() > 0)
	{
		ApproachingTargetID++;
		if (ApproachingTargetID == PatrolTargets.Num()) ApproachingTargetID = 0;
		return PatrolTargets[ApproachingTargetID];
	}
	return this;
}

void AEnemy::OnApproachCompleted()
{
	
}
