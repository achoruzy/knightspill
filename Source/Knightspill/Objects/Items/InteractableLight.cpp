// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableLight.h"

AInteractableLight::AInteractableLight()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableLight::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableLight::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Test"))
}

void AInteractableLight::Interact_Implementation(AMainCharacter* Character)
{
	OnInteract();
}

void AInteractableLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

