// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knightspill/Systems/Interfaces/Interactable.h"
#include "InteractableLight.generated.h"

UCLASS()
class AInteractableLight : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	AInteractableLight();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	bool IsOn = false;
	static void OnInteract();
};
