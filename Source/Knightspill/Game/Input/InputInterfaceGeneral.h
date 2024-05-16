// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "InputInterfaceGeneral.generated.h"

/**
 * 
 */
UCLASS()
class KNIGHTSPILL_API UInputInterfaceGeneral : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MoveForward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MoveSide;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Look;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Jump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Interact;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* WeaponEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AttackLight;
};
