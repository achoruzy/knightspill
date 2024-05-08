// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "ActorExtension.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"

UCLASS()
class KNIGHTSPILL_API ABaseActor : public AActor, public IActorExtension
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="BaseActorDefaults")
	bool bIsSavable = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="BaseActorDefaults")
	FGameplayTagContainer GameplayTags;

private:
	TMap<FString, void*> SavableData;
	
public:
	ABaseActor();
	virtual void Tick(float DeltaTime) override;

	virtual void Save(bool async) override; // TODO: To implement
	virtual void Load(bool async) override; // TODO: To implement
	virtual void RemoveComponent(UActorComponent& comp) override; // TODO: To implement

protected:
	virtual void BeginPlay() override;

private:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
