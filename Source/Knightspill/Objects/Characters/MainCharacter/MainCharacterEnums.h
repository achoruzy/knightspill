// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "MainCharacterEnums.generated.h"

UENUM(BlueprintType)
enum class ECharacterWeaponState : uint8
{
	Unequipped,
	Equipped,
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	Unoccupied,
	Attacking,
	Jumping,
	Animating
};

struct FCharacterSockets
{
	inline static FName RHandSocket = "RHandSocket";
};