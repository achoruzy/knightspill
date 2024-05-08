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
	bool bIsLookingFor;

private:
	AActor* LookAtActor;
	AWeapon* RHandEquipped;
	TArray<AItem*> CollectedItems;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category="Input", meta = (DisplayPriority = 0))
	UInputInterfaceGeneral* InputInterface;
	UPROPERTY(EditAnywhere, Category="Input", meta = (DisplayPriority = 0))
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;
	
public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsWeaponEquipped() const;
	void AttachWeapon(AWeapon* Weapon);
	void CollectItem(AItem* Item);
	void SetCanLookFor(bool var);
	
protected:
	virtual void BeginPlay() override;

private:
	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveSide(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);
};
