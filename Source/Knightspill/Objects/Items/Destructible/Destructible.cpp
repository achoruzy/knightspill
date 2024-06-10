// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Destructible.h"

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Knightspill/Objects/Items/CollectibleContainer.h"


ADestructible::ADestructible()
{
	PrimaryActorTick.bCanEverTick = false;
	BreakableGeometry = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BreakableMesh"));
	RootComponent = BreakableGeometry;
	BreakableGeometry->SetGenerateOverlapEvents(true);
	BreakableGeometry->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->SetupAttachment(RootComponent);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
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
	if (IsBroken) return;
	IsBroken = true;
	if (!SpawnTreasures.IsEmpty())
	{
		const int32 Chance = UKismetMathLibrary::RandomBoolWithWeight(TreasureChance);
		if (!Chance) return;
		const int32 Selection = FMath::RandRange(0, SpawnTreasures.Num() - 1);
		const auto Container = GetWorld()->SpawnActorDeferred<ACollectibleContainer>(ContainerClass, FTransform(GetActorRotation(), GetActorLocation()));
		Container->SetItem(SpawnTreasures[Selection]);
		UGameplayStatics::FinishSpawningActor(Container, FTransform(GetActorRotation(), GetActorLocation()));
	}
}
