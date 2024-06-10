// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Knightspill/Game/EngineClassExtensions/BaseActor.h"
#include "Knightspill/Systems/Interfaces/Collectible.h"

#include "CollectibleContainer.generated.h"

UCLASS()
class KNIGHTSPILL_API ACollectibleContainer : public ABaseActor, public ICollectible
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bHasOwnMesh;
	UPROPERTY(BlueprintReadOnly)
	bool bIsEmpty;
	// TODO: BP editable offset and rotation for mesh to add here

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* HighlightOverlayMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItem> ItemClass;
	UPROPERTY()
	AItem* Item;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* Vfx;
	
public:
	ACollectibleContainer();
	virtual void Tick(float DeltaTime) override;
	virtual void Collect_Implementation(AMainCharacter* Character) override;
	UFUNCTION(BlueprintCallable)
	void ToggleHighlight(bool var);
	void SetItem(const TSubclassOf<AItem>& Item);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// EVENTS
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemCollected);
	UPROPERTY(BlueprintAssignable)
	FItemCollected ItemCollected;
};
