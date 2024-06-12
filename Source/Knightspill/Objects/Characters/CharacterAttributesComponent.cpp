// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CharacterAttributesComponent.h"


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
}

float UCharacterAttributesComponent::GetHealth() const
{
	return Health;
}

float UCharacterAttributesComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void UCharacterAttributesComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

bool UCharacterAttributesComponent::IsAlive() const
{
	return Health > 0.f;
}

