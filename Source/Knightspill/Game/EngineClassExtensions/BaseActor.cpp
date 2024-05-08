// Copyright (C) Data Verft Arkadiusz Choruzy


#include "BaseActor.h"


ABaseActor::ABaseActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseActor::Save(bool async)
{
}

void ABaseActor::Load(bool async)
{
}

void ABaseActor::RemoveComponent(UActorComponent& comp)
{
}

void ABaseActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
}
