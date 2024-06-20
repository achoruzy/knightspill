// Copyright (C) Data Verft Arkadiusz Choruzy


#include "DefaultCharacter.h"

#include "CharacterAttributesComponent.h"
#include "Components/CapsuleComponent.h"
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

float ADefaultCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CharacterAttributes->ReceiveDamage(DamageAmount);
	if (!CharacterAttributes->IsAlive())
	{
		Die();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ADefaultCharacter::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(false);
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

bool ADefaultCharacter::IsAlive() const
{
	return CharacterAttributes->IsAlive();
}

void ADefaultCharacter::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition,
                                              const FVector& DamageNormal)
{
}
