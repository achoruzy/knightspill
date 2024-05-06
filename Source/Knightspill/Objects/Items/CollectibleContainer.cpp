// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CollectibleContainer.h"
#include "Knightspill/Systems/Interfaces/Attachable.h"
#include "Knightspill/Objects/Items/Item.h"
#include "Knightspill/Objects/Items/Equipment/Weapons/Weapon.h"


ACollectibleContainer::ACollectibleContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	Collider->SetupAttachment(GetRootComponent());
	Collider->SetSphereRadius(150.f);
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACollectibleContainer::OnColliderBeginOverlap);

	IsItemVisible = true;
}

void ACollectibleContainer::BeginPlay()
{
	Super::BeginPlay();
	if (ItemClass)
	{
		Item = Cast<AItem>(GetWorld()->SpawnActor(ItemClass, &GetActorTransform()));
		if (Item)
		{
			Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

			if (!IsItemVisible) Item->SetActorHiddenInGame(true);

			if (!MeshRepresentation)
			{
				Item->SetActorHiddenInGame(false);
			}
		}
	}
}

void ACollectibleContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectibleContainer::Collect_Implementation(AMainCharacter* Character)
{

	if (Item)
	{
		if (Item->GetClass()->ImplementsInterface(UAttachable::StaticClass()))
		{
			if (auto attachable = Cast<AWeapon>(Item))
			{
				Character->AttachWeapon(attachable);
				Character->CollectItem(attachable);
			}
			// handle this
			Destroy();
		}
		else if (Item->GetClass()->ImplementsInterface(UCollectible::StaticClass()))
		{
			if (auto collectable = Cast<AItem>(Item))
			{
				Character->CollectItem(collectable);
			}
			// handle this
			Destroy();
		}	
	}
}

void ACollectibleContainer::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* character = Cast<AMainCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, "Test!");
	}
}
