// Copyright (C) Data Verft Arkadiusz Choruzy


#include "Item.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

