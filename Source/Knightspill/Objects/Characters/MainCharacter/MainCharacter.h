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

struct FCharacterSockets
{
	inline static FName RHandSocket = "RHandSocket";
};

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
	virtual void ResetState() override { State = ECharacterActionState::Unoccupied; }
	
protected:
	virtual void BeginPlay() override;
	void AttackLight();

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
	/** Anim callbacks */
	void OnWeaponEquip(const FInputActionValue& Value);
	void OnAttackLight(const FInputActionValue& Value);
};
