// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "ActorExtension.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class KNIGHTSPILL_API ABaseCharacter : public ACharacter, public IActorExtension
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
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void Save(bool async) override; // TODO: To implement - copy from BaseActor implementation
	virtual void Load(bool async) override; // TODO: To implement - copy from BaseActor implementation
	virtual void RemoveComponent(UActorComponent& comp) override; // TODO: To implement - copy from BaseActor implementation

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PlayAnimationMontage(UAnimMontage* Montage) const;
	virtual void PlayAnimationMontage(UAnimMontage* Montage, const FName& Section) const;

private:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
