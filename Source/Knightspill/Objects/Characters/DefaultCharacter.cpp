// Copyright (C) Data Verft Arkadiusz Choruzy


#include "DefaultCharacter.h"

#include "Knightspill/Objects/Items/Equipment/Weapons/Weapon.h"


ADefaultCharacter::ADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADefaultCharacter::StartHit()
{
	WeaponEquipped->SetActive(true);
}

void ADefaultCharacter::StopHit()
{
	WeaponEquipped->SetActive(false);
	if (AWeapon* Weapon = Cast<AWeapon>(WeaponEquipped))
	{
		Weapon->ResetActorsToIgnore();
	}
}

void ADefaultCharacter::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition,
	const FVector& DamageNormal)
{
}
