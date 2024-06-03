// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Weapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	// IsEquipped = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	WeaponBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxCollider"));
	WeaponBoxCollider->SetupAttachment(RootComponent);
	WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxCollider->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	WeaponBoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
	HitTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("HitTraceStart"));
	HitTraceStart->SetupAttachment(RootComponent);
	HitTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("HitTraceEnd"));
	HitTraceEnd->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Attach_Implementation(AMainCharacter* Character)
{
}

void AWeapon::Collect_Implementation(AMainCharacter* Character)
{
}

void AWeapon::SetActive(bool value)
{
	// IsEquipped = value;
	if (value)
		WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (!IsEquipped) return;
	const FVector Start = HitTraceStart->GetComponentLocation();
	const FVector End = HitTraceEnd->GetComponentLocation();
	const FVector HalfSize {2.5f, 2.5f, 2.5f};
	const TArray<AActor*> ActorsToIgnore {this, this->GetOwner()};
	FHitResult HitData;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		HalfSize,
		HitTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitData,
		true);

	if (const auto HittableActor = Cast<IHittable>(HitData.GetActor()))
	{
		HittableActor->GetHit_Implementation(Damage, HitData.Location, HitData.Normal);
	}
}
