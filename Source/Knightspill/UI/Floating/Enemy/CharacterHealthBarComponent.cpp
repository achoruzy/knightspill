// Copyright (C) Data Verft Arkadiusz Choruzy


#include "CharacterHealthBarComponent.h"


UCharacterHealthBarComponent::UCharacterHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

