// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Enemy.h"

#include "Components/CapsuleComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
		UE_LOG(LogTemp, Warning, TEXT("Got Hit from front"));
	}
	else if (Theta <= -45.f && Theta >= -135.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got Hit from left"));
	}
	else if (Theta >= 45.f && Theta <= -135.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got Hit from right"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Got Hit from back"));
	}
}
