// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "EnemyEnums.generated.h"

UENUM(BlueprintType)
enum class EEnemyLivingStatus : uint8
{
	Lives,
	DiedForward,
	DiedBack
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Wait,
	Idle,
	Patrol,
	Chase,
	Attack
};

struct FEnemyAttackMontageTitles
{
	inline static FName LightAttack = "LightAttack";
};