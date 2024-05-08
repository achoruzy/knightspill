// Copyright (C) Data Verft Arkadiusz Choruzy.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainCharacter.generated.h"

class AItem;
class AWeapon;
class UInputInterfaceGeneral;
struct FInputActionValue;
class UInputMappingContext;

UCLASS()
class KNIGHTSPILL_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category="Input", meta = (DisplayPriority = 0))
	UInputInterfaceGeneral* InputInterface;
	UPROPERTY(EditAnywhere, Category="Input", meta = (DisplayPriority = 0))
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	AActor* LookAtActor;
	bool bIsLookingFor;
	
	AWeapon* RHandEquipped;
	TArray<AItem*> CollectedItems;

	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveSide(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);

public:
	bool IsWeaponEquipped() const;
	void AttachWeapon(AWeapon* Weapon);
	void CollectItem(AItem* Item);
	void SetCanLookFor(bool var);
};
