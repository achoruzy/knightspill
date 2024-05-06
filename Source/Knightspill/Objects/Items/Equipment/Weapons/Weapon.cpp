// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Weapon.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
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

