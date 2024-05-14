// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "MainCharacterEnums.generated.h"

UENUM(BlueprintType)
enum class ECharacterActiveEquipmentState : uint8
{
	Unequipped,
	RightHandWeapon,
	Shield,
	RightHandWeaponAndShield
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	Unoccupied,
	Attacking,
	Jumping
};