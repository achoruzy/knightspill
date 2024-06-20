// Copyright (C) Data Verft Arkadiusz Choruzy.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Knightspill/Objects/Characters/DefaultCharacter.h"
#include "Knightspill/Objects/Characters/MainCharacter/MainCharacterEnums.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AItem;
class AWeapon;
class UInputInterfaceGeneral;
struct FInputActionValue;
class UInputMappingContext;
class UAnimMontage;

UCLASS()
class KNIGHTSPILL_API AMainCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category="! Input")
	UInputInterfaceGeneral* InputInterface;
	UPROPERTY(EditAnywhere, Category="! Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;
	
	TArray<AItem*> CollectedItems; // TODO: Refactor to backpack component
	UPROPERTY()
	AActor* SeenInteractable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"), Category="! States")
	ECharacterWeaponState WeaponState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"), Category="! States")
	ECharacterActionState State;
	
	bool bCanTrace;
	
	//** ANIM MONTAGES */
	UPROPERTY(EditDefaultsOnly, Category="! AnimMontages")
	UAnimMontage* WeaponEquipMontage;

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsWeaponEquipped() const;
	UFUNCTION(BlueprintPure)
	bool IsBusy() const;
	void AttachWeapon(AWeapon* Weapon);
	void CollectItem(AItem* Item);
	void SetCanTrace(bool CanTrace);

	UFUNCTION(BlueprintPure)
	FORCEINLINE ECharacterWeaponState GetWeaponState() const { return WeaponState; }
	/** DefaultCharacter */
	virtual void ResetState() override { State = ECharacterActionState::Unoccupied; }
	
protected:
	virtual void BeginPlay() override;
	void AttackLight();
	/** DefaultCharacter */
	virtual void Die() override;
	/** IHittable */
	virtual void GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal) override;

private:
	void TraceLine();
	bool CanAttack() const;
	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = "true"))
	void ArmWeapon();
	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = "true"))
	void DisarmWeapon();
	/** Input */
	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveSide(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);
	void OnWeaponEquip(const FInputActionValue& Value);
	void OnAttackLight(const FInputActionValue& Value);
};
