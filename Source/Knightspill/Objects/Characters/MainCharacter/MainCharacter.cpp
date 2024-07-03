// Copyright (C) Data Verft Arkadiusz Choruzy


#include "MainCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Knightspill/Game/Input/InputInterfaceGeneral.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Knightspill/Systems/Interfaces/Collectible.h"
#include "Knightspill/Systems/Interfaces/Interactable.h"
#include "Knightspill/Objects/Items/Equipment/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Knightspill/Game/HUD/KnightspillHUD.h"
#include "Knightspill/Objects/Characters/CharacterAttributesComponent.h"
#include "Knightspill/Objects/Items/Collectibles/OverlapCollectible.h"
#include "Knightspill/UI/HUD/HUDOverlay.h"


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPPSpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
	Camera->SetupAttachment(SpringArm);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->bUseControllerDesiredRotation = true;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Knightspill.Actions.EnemyHittable"));

	bCanTrace = false;
	WeaponState = ECharacterWeaponState::Unequipped;
	State = ECharacterActionState::Unoccupied;

	CharacterAttributes->HealHealth(200.f);
	CharacterAttributes->HealthUpdated.AddLambda([this] (const float NewPrc) { HUD->GetOverlay()->SetHealthPercent(NewPrc); });
	CharacterAttributes->StaminaUpdated.AddLambda([this] (const float NewPrc) { HUD->GetOverlay()->SetStaminaPercent(NewPrc); });
	CharacterAttributes->SoulsUpdated.AddLambda([this] (const int NewVal) { HUD->GetOverlay()->SetSoulsValue(NewVal); });
	CharacterAttributes->GoldUpdated.AddLambda([this] (const int NewVal) { HUD->GetOverlay()->SetGoldValue(NewVal); });

	HUD = Cast<AKnightspillHUD>(Cast<APlayerController>(GetController())->GetHUD());

	HUD->GetOverlay()->SetHealthPercent(1.f);
	HUD->GetOverlay()->SetStaminaPercent(1.f);
	HUD->GetOverlay()->SetSoulsValue(0);
	HUD->GetOverlay()->SetGoldValue(0);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanTrace) TraceLine();
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (InputMappingContext.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Err: Input mapping context not attached to MainCharacter!"));
		return;
	}
	
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 0);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	/* Input bindings here*/
	Input->BindAction(InputInterface->MoveForward, ETriggerEvent::Triggered, this, &AMainCharacter::OnMoveForward);
	Input->BindAction(InputInterface->MoveSide, ETriggerEvent::Triggered, this, &AMainCharacter::OnMoveSide);
	Input->BindAction(InputInterface->Look, ETriggerEvent::Triggered, this, &AMainCharacter::OnLook);
	Input->BindAction(InputInterface->Jump, ETriggerEvent::Triggered, this, &AMainCharacter::OnJump);
	Input->BindAction(InputInterface->Interact, ETriggerEvent::Triggered, this, &AMainCharacter::OnInteract);
	Input->BindAction(InputInterface->WeaponEquip, ETriggerEvent::Triggered, this, &AMainCharacter::OnWeaponEquip);
	Input->BindAction(InputInterface->AttackLight, ETriggerEvent::Triggered, this, &AMainCharacter::OnAttackLight);
}

bool AMainCharacter::IsWeaponEquipped() const
{
	if (WeaponState != ECharacterWeaponState::Unequipped) return true;
	return false;
}

bool AMainCharacter::IsBusy() const
{
	if (State == ECharacterActionState::Unoccupied) return false;
	return true;
}

void AMainCharacter::AttachWeapon(AWeapon* NewWeapon)
{
	if (!Weapon)
	{
		Weapon = NewWeapon;
		Weapon->Equip(GetMesh(), this, FCharacterSockets::RHandSocket);
		WeaponState = ECharacterWeaponState::Equipped;
		Weapon->ResetActorsToIgnore();
	}
}

void AMainCharacter::AttackLight()
{
	State = ECharacterActionState::Attacking;
	CharacterAttributes->UseStamina(AttackStaminaCost);
	PlayAnimationMontage(AttackMontage);
}

void AMainCharacter::Die()
{
	Super::Die();
	State = ECharacterActionState::Dead;
	PlayAnimationMontage(DeathMontage);
	DisableInput(GetWorld()->GetFirstPlayerController());
}

void AMainCharacter::GetHit_Implementation(const int DamageValue, const FVector& DamagePosition, const FVector& DamageNormal)
{
	if (!IsAlive()) return;

	State = ECharacterActionState::Animating;
	
	const FVector Forward = GetActorForwardVector();
	const FVector ToHitPos = (DamagePosition - GetActorLocation()).GetSafeNormal();
	const FVector FlattenHitPos(ToHitPos.X, ToHitPos.Y, Forward.Z);
	const double CosTheta = FVector::DotProduct(Forward, FlattenHitPos);
	const double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
	if (Theta > -45.f && Theta < 45.f)
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactFront"));
	}
	else if (Theta <= -45.f && Theta >= -135.f)
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactLeft"));
	}
	else if (Theta >= 45.f && Theta <= -135.f)
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactRight"));
	}
	else
	{
		PlayAnimationMontage(HitReactionMontage, FName("HitReactBack"));
	}
}

void AMainCharacter::CollectItem(AItem* Item)
{
	if (const AOverlapCollectible* Collectible = Cast<AOverlapCollectible>(Item))
	{
		if (Item->Tags.Contains(FName("Knightspill.Collectibles.Health")))
		{
			CharacterAttributes->HealHealth(Collectible->GetValue());
			HUD->GetOverlay()->SetHealthPercent(CharacterAttributes->GetHealthPercent());
		}
		if (Item->Tags.Contains(FName("Knightspill.Collectibles.Souls")))
		{
			CollectSouls(Collectible->GetValue());
		}
		if (Item->Tags.Contains(FName("Knightspill.Collectibles.Gold")))
		{
			CollectGold(Collectible->GetValue());
		}
	}
	else CollectedItems.Add(Item);
}

void AMainCharacter::SetCanTrace(const bool CanTrace)
{
	bCanTrace = CanTrace;
	if (!bCanTrace) SeenInteractable = nullptr;
}

void AMainCharacter::CollectSouls(const int Souls) const
{
	CharacterAttributes->AddSouls(Souls);
}

void AMainCharacter::CollectGold(const int Gold) const
{
	CharacterAttributes->AddGold(Gold);
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	const float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HUD->GetOverlay()->SetHealthPercent(CharacterAttributes->GetHealthPercent());
	return Result;
}

void AMainCharacter::TraceLine()
{
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceEnd = Camera->GetComponentLocation() + Camera->GetForwardVector() * 400.f;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.MobilityType = EQueryMobilityType::Static;
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		SeenInteractable = Hit.GetActor();
	}
	else
	{
		SeenInteractable = nullptr;
	}
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, -1, 0, 0.1f);
}

bool AMainCharacter::CanAttack() const
{
	return !IsBusy() && IsWeaponEquipped() && (CharacterAttributes->GetStamina() >= AttackStaminaCost);
}

void AMainCharacter::ArmWeapon()
{
	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RHandSocket"));
		Weapon->SetActive(true);
		WeaponState = ECharacterWeaponState::Equipped;
	}
}

void AMainCharacter::DisarmWeapon()
{
	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("SwordSlotBack"));
		Weapon->SetActive(false);
		WeaponState = ECharacterWeaponState::Unequipped;
	}
}

void AMainCharacter::OnMoveForward(const FInputActionValue& Value)
{
	if (const float Val = Value.Get<float>(); Val != 0.0)
	{
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void AMainCharacter::OnMoveSide(const FInputActionValue& Value)
{
	if (const float Val = Value.Get<float>(); Val != 0.0)
	{
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}

void AMainCharacter::OnLook(const FInputActionValue& Value)
{
	if (const FVector2D Val = Value.Get<FVector2D>(); !Val.IsZero())
	{
		AddControllerPitchInput(-Val.Y);
		AddControllerYawInput(Val.X);
	}
}

void AMainCharacter::OnJump(const FInputActionValue& Value)
{
	Jump();
}

void AMainCharacter::OnInteract(const FInputActionValue& Value)
{
	if (SeenInteractable)
	{
		if (auto collectible = Cast<ICollectible>(SeenInteractable))
		{
			collectible->Collect_Implementation(this);
		}
		else if (auto interactable = Cast<IInteractable>(SeenInteractable))
		{
			interactable->Interact_Implementation(this);
		}
	}
}

void AMainCharacter::OnWeaponEquip(const FInputActionValue& Value)
{
	if (Weapon && !IsBusy())
	{
		if (IsWeaponEquipped())
		{
			State = ECharacterActionState::Animating;
			PlayAnimationMontage(WeaponEquipMontage, FName("Unequip"));
		}
		else
		{
			State = ECharacterActionState::Animating;
			PlayAnimationMontage(WeaponEquipMontage, FName("Equip"));
		}
	}
}

void AMainCharacter::OnAttackLight(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		AttackLight();
	}
}
