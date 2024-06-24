// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Objects/Items/Item.h"
#include "OverlapCollectible.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;

UCLASS()
class KNIGHTSPILL_API AOverlapCollectible : public AItem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="! Overlap Collectible", meta=(AllowPrivateAccess="true"))
	float OverlapRadius = 100;
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* OverlapSphere;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* CollectibleParticleSystem;
	UPROPERTY(EditDefaultsOnly, Category="! Overlap Collectible")
	UNiagaraSystem* OnCollectedParticleSystem;
	UPROPERTY(EditDefaultsOnly, Category="! Overlap Collectible", meta=(AllowPrivateAccess="true"))
	FGameplayTag CollectibleTag;
	UPROPERTY(EditAnywhere, Category="! Overlap Collectible", meta=(AllowPrivateAccess="true"))
	int Value = 0;
	UPROPERTY(EditAnywhere, Category="! Overlap Collectible", meta=(AllowPrivateAccess="true"))
	USoundBase* PickupSound;
	
public:
	AOverlapCollectible();
	virtual void Tick(float DeltaTime) override;
	virtual void Collect_Implementation(AMainCharacter* Character) override;
	void SetValue(const int NewVal) { Value = NewVal; }
	int GetValue() const { return Value; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void GetCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
