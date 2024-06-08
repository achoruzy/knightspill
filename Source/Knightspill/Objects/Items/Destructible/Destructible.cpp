// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Destructible.h"

#include "GeometryCollection/GeometryCollectionComponent.h"


ADestructible::ADestructible()
{
	PrimaryActorTick.bCanEverTick = false;
	BreakableGeometry = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BreakableMesh"));
	RootComponent = BreakableGeometry;
	BreakableGeometry->SetGenerateOverlapEvents(true);
	BreakableGeometry->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADestructible::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructible::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition,
	const FVector& DamageNormal)
{
}
