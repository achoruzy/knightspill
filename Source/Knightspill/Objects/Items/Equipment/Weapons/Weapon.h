// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Knightspill/Objects/Items/Item.h"
#include "Knightspill/Systems/Interfaces/Attachable.h"
#include "Knightspill/Systems/Interfaces/Collectible.h"

#include "Weapon.generated.h"

UCLASS()
class KNIGHTSPILL_API AWeapon : public AItem, public IAttachable, public ICollectible
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;
	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Attach_Implementation(AMainCharacter* Character) override;
	virtual void Collect_Implementation(AMainCharacter* Character) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	int Damage = 0;
	
};
