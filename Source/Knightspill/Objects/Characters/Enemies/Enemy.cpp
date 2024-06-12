// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/UI/Floating/Enemy/CharacterHealthBarComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributesComponent>(TEXT("ArrtibutesComponent"));

	HealthBarComponent = CreateDefaultSubobject<UCharacterHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealthBarComponent->SetHealthPercent(1.f);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CharacterAttributes->ReceiveDamage(DamageAmount);
	HealthBarComponent->SetHealthPercent(CharacterAttributes->GetHealthPercent());
	
	if (!CharacterAttributes->IsAlive())
	{
		Die();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemy::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal)
{
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

void AEnemy::Die() const
{
	PlayAnimationMontage(DeathMontage);
	HealthBarComponent->SetVisibility(false);
	// GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	// GetMesh()->SetGenerateOverlapEvents(false);
	// GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}
