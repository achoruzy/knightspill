// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributesComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KNIGHTSPILL_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, Category="! Character Attributes")
	float Health;
	UPROPERTY(EditAnywhere, Category="! Character Attributes")
	float MaxHealth;

public:
	UCharacterAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetHealth() const;
	void SetHealth(const float NewHealth);
	float GetHealthPercent() const;
	void ReceiveDamage(const float Damage);
	bool IsAlive() const;

protected:
	virtual void BeginPlay() override;
};
