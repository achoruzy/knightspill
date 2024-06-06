// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CollectibleContainer.h"
#include "Knightspill/Systems/Interfaces/Attachable.h"
#include "Knightspill/Objects/Items/Item.h"
#include "Knightspill/Objects/Items/Equipment/Weapons/Weapon.h"


ACollectibleContainer::ACollectibleContainer()
{
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	RootComponent = Collider;
	Collider->SetSphereRadius(150.f);
	Collider->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ACollectibleContainer::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACollectibleContainer::OnColliderBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ACollectibleContainer::OnColliderEndOverlap);
	
	if (ItemClass)
	{
		Item = Cast<AItem>(GetWorld()->SpawnActor(ItemClass, &GetActorTransform()));
		if (Item)
		{
			Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

			if (Mesh->GetStaticMesh())
			{
				Item->SetActorHiddenInGame(true);
				bHasOwnMesh = true;
			}
			else
			{
				Item->SetActorHiddenInGame(true);
				auto comp = Item->GetComponentByClass(UStaticMeshComponent::StaticClass());
				if (auto itemMeshComp = Cast<UStaticMeshComponent>(comp))
				{
					Mesh->SetStaticMesh(itemMeshComp->GetStaticMesh());
					Mesh->AttachToComponent(Collider, FAttachmentTransformRules::SnapToTargetIncludingScale);
					bHasOwnMesh = false;
				}
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
	TArray<AActor*> OverlappedCharacters;
	Collider->GetOverlappingActors(OverlappedCharacters, AMainCharacter::StaticClass());
	if (Item && (!OverlappedCharacters.IsEmpty()))
	{
	UE_LOG(LogTemp, Warning, TEXT("Collects on: %s"), *GetName());
		if (Item->GetClass()->ImplementsInterface(UAttachable::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Collecting %s"), *Item->GetName());
			if (auto attachable = Cast<AWeapon>(Item))
			{
				if (!bHasOwnMesh)
				{
					Mesh->SetStaticMesh(nullptr);
				}
				attachable->SetActorHiddenInGame(false);
				Character->AttachWeapon(attachable);
				Character->CollectItem(Item);
				Destroy(); // as the item was taken and nothing left
			}
		}
		else if (Item->GetClass()->ImplementsInterface(UCollectible::StaticClass()))
		{
			if (auto collectable = Cast<AItem>(Item))
			{
				Character->CollectItem(collectable);
				// destroy not needed as prop stays on level
			}
		}	
	}
}

void ACollectibleContainer::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* character = Cast<AMainCharacter>(OtherActor))
	{
		character->SetCanLookFor(true);
		UE_LOG(LogTemp, Warning, TEXT("Character got into collider"));
		ToggleHighlight(true);
	}
}

void ACollectibleContainer::OnColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMainCharacter* character = Cast<AMainCharacter>(OtherActor))
	{
		character->SetCanLookFor(false);
		UE_LOG(LogTemp, Warning, TEXT("Character got out of the collider"));
		ToggleHighlight(false);
	}
}

void ACollectibleContainer::ToggleHighlight(bool var)
{
	if (!HighlightOverlayMaterial) return;
	if (var) Mesh->SetOverlayMaterial(HighlightOverlayMaterial);
	else Mesh->SetOverlayMaterial(nullptr);
}
