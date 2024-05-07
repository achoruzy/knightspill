// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Knightspill/Systems/Interfaces/Collectible.h"

#include "CollectibleContainer.generated.h"

UCLASS()
class KNIGHTSPILL_API ACollectibleContainer : public AActor, public ICollectible
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* Collider;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> ItemClass;
	AItem* Item;
	UPROPERTY(EditAnywhere)
	bool HasOwnMesh;

	ACollectibleContainer();
	virtual void Tick(float DeltaTime) override;
	virtual void Collect_Implementation(AMainCharacter* Character) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
