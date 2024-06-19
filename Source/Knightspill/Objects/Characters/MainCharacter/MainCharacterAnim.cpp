// Copyright (C) Data Verft Arkadiusz Choruzy


#include "MainCharacterAnim.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AMainCharacter>(TryGetPawnOwner());
	if (Owner)
	{
		MovementComponent = Owner->GetCharacterMovement();
	}
	
}

void UMainCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (MovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		IsFalling = MovementComponent->IsFalling();
		ActiveEquipmentState = Owner->GetWeaponState();
	}
}
