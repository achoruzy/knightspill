// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributesComponent.generated.h"


class AMainCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KNIGHTSPILL_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, Category="! Character Attributes")
	float Health;
	UPROPERTY(EditAnywhere, Category="! Character Attributes")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category="! Character Attributes")
	float HealthRegenValue;
	UPROPERTY(EditAnywhere, Category="! Character Attributes")
	float HealthRegenInterval;
	float HealthRegenIntervalCurrent;
	UPROPERTY(EditInstanceOnly, Category="! Character Attributes")
	float Stamina;
	UPROPERTY(EditAnywhere, Category="! Character Attributes")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category="! Character Attributes")
	float StaminaRegenValue;
	UPROPERTY(EditAnywhere, Category="! Character Attributes")
	float StaminaRegenInterval;
	float StaminaRegenIntervalCurrent;
	UPROPERTY(EditInstanceOnly, Category="! Character Attributes")
	int Souls;
	UPROPERTY(EditInstanceOnly, Category="! Character Attributes")
	int Gold;

public:
	UCharacterAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetHealth() const;
	void HealHealth(const float AddHealth);
	float GetHealthPercent() const;
	void ReceiveDamage(const float Damage);
	bool IsAlive() const;
	void HealStamina(const int AddStamina);
	void UseStamina(const int UseStamina);
	float GetStaminaPercent() const;
	int GetStamina() const;
	void AddSouls(int SoulsToAdd);
	void UseSouls(int SoulsToUse);
	int GetSouls() const { return Souls; }
	void AddGold(int GoldToAdd);
	void UseGold(int GoldToUse);
	int GetGold() const { return Gold; }

	/** DISPATCHERS */
	DECLARE_MULTICAST_DELEGATE_OneParam(FHealthUpdated, float);
	FHealthUpdated HealthUpdated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FStaminaUpdated, float);
	FHealthUpdated StaminaUpdated;
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FSoulsUpdated, int);
	FSoulsUpdated SoulsUpdated;
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FGoldUpdated, int);
	FGoldUpdated GoldUpdated;

protected:
	virtual void BeginPlay() override;
};
