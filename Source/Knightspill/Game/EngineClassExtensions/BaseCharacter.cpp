// Copyright (C) Data Verft Arkadiusz Choruzy


#include "BaseCharacter.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Save(bool async)
{
}

void ABaseCharacter::Load(bool async)
{
}

void ABaseCharacter::RemoveComponent(UActorComponent& comp)
{
}


void ABaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
}

void ABaseCharacter::PlayAnimationMontage(UAnimMontage* Montage) const
{
	int32 sectionNum = FMath::RandRange(0, Montage->GetNumSections() - 1);
	FName section = Montage->GetSectionName(sectionNum);
	PlayAnimationMontage(Montage, section);
}

void ABaseCharacter::PlayAnimationMontage(UAnimMontage* Montage, const FName& Section) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(Section, Montage);
	}
}

