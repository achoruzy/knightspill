// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Systems/Interfaces/Attachable.h"

#include "Weapon.generated.h"

UCLASS()
class KNIGHTSPILL_API AWeapon : public AActor, public IAttachable
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;
	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Attach_Implementation(AMainCharacter* Character) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	int Damage = 0;
	
};
