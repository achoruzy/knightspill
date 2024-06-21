// Copyright (C) Data Verft Arkadiusz Choruzy


#include "DefaultCharacter.h"

#include "CharacterAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "Knightspill/Objects/Items/Equipment/Weapons/Weapon.h"



ADefaultCharacter::ADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributesComponent>(TEXT("ArrtibutesComponent"));
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
	if (!Weapon) return;
	Weapon->SetActive(true);
}

void ADefaultCharacter::StopHit()
{
	if (!Weapon) return;
	Weapon->SetActive(false);
	Weapon->ResetActorsToIgnore();
}

bool ADefaultCharacter::IsAlive() const
{
	return CharacterAttributes->IsAlive();
}

void ADefaultCharacter::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition,
                                              const FVector& DamageNormal)
{
	
}
