// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainCharacter.generated.h"

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

	void MoveForward(const FInputActionValue& Value);
	void MoveSide(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpAction(const FInputActionValue& Value);
};
