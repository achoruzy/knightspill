// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CharacterHealthBarComponent.h"
#include "CharacterHealthBar.h"


UCharacterHealthBarComponent::UCharacterHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
	HealthBar = Cast<UCharacterHealthBar>(GetUserWidgetObject());
}

void UCharacterHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterHealthBarComponent::SetHealthPercent(const float Percent)
{
	if (HealthBar) HealthBar->SetHealthPercent(Percent);
}

