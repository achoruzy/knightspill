// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Weapon.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Knightspill/Systems/Interfaces/Hittable.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	// IsEquipped = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	StaticMesh->SetGenerateOverlapEvents(false);

	WeaponBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxCollider"));
	WeaponBoxCollider->SetupAttachment(RootComponent);
	WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxCollider->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	WeaponBoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponBoxCollider->SetGenerateOverlapEvents(true);
	
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

void AWeapon::SetActive(bool value)
{
	// IsEquipped = value;
	if (value)
		WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
	{
		WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void AWeapon::Equip(USceneComponent* Parent, AActor* WeaponOwner, FName SocketName)
{
	const auto Rules = FAttachmentTransformRules::SnapToTargetIncludingScale;
	AttachToComponent(Parent, Rules, SocketName);
	SetOwner(WeaponOwner);
	SetInstigator(Cast<APawn>(WeaponOwner));
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;
	
	const FVector Start = HitTraceStart->GetComponentLocation();
	const FVector End = HitTraceEnd->GetComponentLocation();
	const FVector HalfSize {2.5f, 2.5f, 2.5f};
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
		EDrawDebugTrace::None,
		HitData,
		true);

	if (const auto HittableActor = Cast<IHittable>(HitData.GetActor()))
	{
		HittableActor->Execute_GetHit(HitData.GetActor(), Damage, HitData.Location, HitData.Normal);
		ActorsToIgnore.AddUnique(HitData.GetActor());
		UGameplayStatics::ApplyDamage(HitData.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
	ApplyForceFields(HitData.Location);
}

void AWeapon::ResetActorsToIgnore()
{
	ActorsToIgnore.Empty();
	ActorsToIgnore.AddUnique(this);
	ActorsToIgnore.AddUnique(this->Owner);
}
