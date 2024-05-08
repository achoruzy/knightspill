// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "ActorExtension.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UActorExtension : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KNIGHTSPILL_API IActorExtension: public IGameplayTagAssetInterface
{
	GENERATED_BODY()

private:
	TMap<FString, void*> SavableData;
	
public:
	virtual void Save(bool async) = 0;
	virtual void Load(bool async) = 0;
	virtual void RemoveComponent(UActorComponent& comp) = 0;
};
