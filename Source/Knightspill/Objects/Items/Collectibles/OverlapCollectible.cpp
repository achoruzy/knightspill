// Copyright (C) Data Verft Arkadiusz Choruzy


#include "OverlapCollectible.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AOverlapCollectible::AOverlapCollectible()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetSphereRadius(OverlapRadius);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapSphere->SetGenerateOverlapEvents(true);
	RootComponent = OverlapSphere;

	CollectibleParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle System"));
	CollectibleParticleSystem->SetupAttachment(RootComponent);
}

void AOverlapCollectible::BeginPlay()
{
	Super::BeginPlay();
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AOverlapCollectible::GetCollected);

	Tags.Add(CollectibleTag.GetTagName());
}

void AOverlapCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOverlapCollectible::Collect_Implementation(AMainCharacter* Character)
{
	Super::Collect_Implementation(Character);
}

void AOverlapCollectible::GetCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto MainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		MainCharacter->CollectItem(this);
		if (PickupSound) UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation());
		if (OnCollectedParticleSystem) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OnCollectedParticleSystem, GetActorLocation());
		Destroy();
	}
}
