// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CollectibleContainer.h"


ACollectibleContainer::ACollectibleContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
}

void ACollectibleContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACollectibleContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectibleContainer::Collect_Implementation(AMainCharacter* Character)
{
	if (Item)
	{
		if (Item->Implements<UAttachable>())
		{
			Item->Attach(Character);
			Destroy();
		}	
	}
}

