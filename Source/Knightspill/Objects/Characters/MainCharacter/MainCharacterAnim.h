// Copyright (C) Data Verft Arkadiusz Choruzy

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnim.generated.h"

class UCharacterMovementComponent;
class AMainCharacter;
/**
 * 
 */
UCLASS()
class KNIGHTSPILL_API UMainCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	AMainCharacter* Owner;
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float GroundSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool IsFalling = false;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
};
