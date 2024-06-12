// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Knightspill/Objects/Items/Item.h"
#include "Knightspill/Systems/Interfaces/Attachable.h"
#include "Knightspill/Systems/Interfaces/Collectible.h"

#include "Weapon.generated.h"

UCLASS()
class KNIGHTSPILL_API AWeapon : public AItem, public IAttachable
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

private:
	// UPROPERTY(VisibleAnywhere)
	// bool IsEquipped;

protected:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	int Damage = 35; // TODO: Get out to a component and all other characteristics
	// TODO: setup weapons to data tables

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBoxComponent* WeaponBoxCollider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* HitTraceStart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* HitTraceEnd;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Attach_Implementation(AMainCharacter* Character) override;
	UFUNCTION(BlueprintCallable)
	void SetActive(bool value);
	UFUNCTION()
	void ResetActorsToIgnore();
	UFUNCTION()
	void Equip(USceneComponent* Parent,	AActor* WeaponOwner, FName SocketName);
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyForceFields(const FVector& Location);
private:
};
