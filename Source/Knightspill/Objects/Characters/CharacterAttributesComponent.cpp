// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CharacterAttributesComponent.h"

#include "Knightspill/Game/HUD/KnightspillHUD.h"
#include "MainCharacter/MainCharacter.h"


UCharacterAttributesComponent::UCharacterAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100.f;
	Health = MaxHealth;
}



void UCharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCharacterAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsAlive()) return;
	
	HealthRegenIntervalCurrent += DeltaTime;
	StaminaRegenIntervalCurrent += DeltaTime;

	if (HealthRegenIntervalCurrent > HealthRegenInterval)
	{
		HealHealth(HealthRegenValue);
		HealthRegenIntervalCurrent = 0.f;
	}
	if (StaminaRegenIntervalCurrent > StaminaRegenInterval)
	{
		HealStamina(StaminaRegenValue);
		StaminaRegenIntervalCurrent = 0.f;
	}
}

float UCharacterAttributesComponent::GetHealth() const
{
	return Health;
}

void UCharacterAttributesComponent::HealHealth(const float AddHealth)
{
	Health += AddHealth;
	HealthUpdated.Broadcast(GetHealthPercent());
}

float UCharacterAttributesComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void UCharacterAttributesComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	HealthUpdated.Broadcast(GetHealthPercent());
}

bool UCharacterAttributesComponent::IsAlive() const
{
	return Health > 0.f;
}

void UCharacterAttributesComponent::HealStamina(const int AddStamina)
{
	Stamina = FMath::Clamp(Stamina + AddStamina, 0.f, MaxStamina);
}

void UCharacterAttributesComponent::UseStamina(const int UseStamina)
{
	Stamina = FMath::Clamp(Stamina - UseStamina, 0.f, MaxStamina);
}

float UCharacterAttributesComponent::GetStaminaPercent() const
{
	return Stamina / MaxStamina;
}

int UCharacterAttributesComponent::GetStamina() const
{
	return Stamina;
}

void UCharacterAttributesComponent::UseSouls(const int SoulsToUse)
{
	Souls = FMath::Clamp(Souls - SoulsToUse, 0.f, Souls);
}

void UCharacterAttributesComponent::UseGold(const int GoldToUse)
{
	Gold = FMath::Clamp(Gold - GoldToUse, 0.f, Gold);
}
