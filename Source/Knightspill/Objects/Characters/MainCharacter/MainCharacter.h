// Copyright (C) Data Verft Arkadiusz Choruzy.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Knightspill/Game/EngineClassExtensions/BaseCharacter.h"
#include "Knightspill/Objects/Characters/MainCharacter/MainCharacterEnums.h"
#include "MainCharacter.generated.h"

class AItem;
class AWeapon;
class UInputInterfaceGeneral;
struct FInputActionValue;
class UInputMappingContext;
class UAnimMontage;

struct ECharacterSockets
{
	inline static FName RHandSocket = "RHandSocket";
};

UCLASS()
class KNIGHTSPILL_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	bool bIsLookingFor;

private:
	AActor* LookAtActor;
	AWeapon* RHandEquipped;
	TArray<AItem*> CollectedItems;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	ECharacterActiveEquipmentState ActiveEquipmentState;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category="Input", meta = (DisplayPriority = 0))
	UInputInterfaceGeneral* InputInterface;
	UPROPERTY(EditAnywhere, Category="Input", meta = (DisplayPriority = 1))
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	//** ANIM MONTAGES */
	UPROPERTY(EditDefaultsOnly, Category="AnimMontages")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category="AnimMontages")
	UAnimMontage* WeaponEquipMontage;

	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = "true"))
	void ArmWeapon();
	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = "true"))
	void DisarmWeapon();
	
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
	void SetCanLookFor(bool var);

	UFUNCTION(BlueprintPure)
	FORCEINLINE ECharacterActiveEquipmentState GetActiveEquipmentState() const { return ActiveEquipmentState; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetActionState() { ActionState = ECharacterActionState::Unoccupied; }
	
protected:
	virtual void BeginPlay() override;

private:
	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveSide(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);
	void OnWeaponEquip(const FInputActionValue& Value);
	
	void OnAttackLight(const FInputActionValue& Value);

	void PlayAnimationMontage(UAnimMontage* Montage) const;
	void PlayAnimationMontage(UAnimMontage* Montage, const FName& Section) const;
};
