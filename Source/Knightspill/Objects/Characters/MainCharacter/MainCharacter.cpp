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
#include "Knightspill/Objects/Items/Item.h"
#include "Knightspill/Objects/Items/Equipment/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

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
	Tags.Add(FName("MainCharacter"));

	bIsLookingFor = false;
	ActiveEquipmentState = ECharacterActiveEquipmentState::Unequipped;
	ActionState = ECharacterActionState::Unoccupied;
}


void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Trace line for looking at actor
	if (bIsLookingFor)
	{
		FHitResult Hit;
		FVector TraceStart = Camera->GetComponentLocation();
		FVector TraceEnd = Camera->GetComponentLocation() + Camera->GetForwardVector() * 400.f;
		FCollisionQueryParams QueryParams; // TODO: get these 3 lines out
		QueryParams.AddIgnoredActor(this);
		QueryParams.MobilityType = EQueryMobilityType::Static;
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		if (Hit.bBlockingHit && IsValid(Hit.GetActor())) LookAtActor = Hit.GetActor();
		else LookAtActor = nullptr;
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, -1, 0, 0.1f);
	}
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
	if (LookAtActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *LookAtActor->GetName());
		if (auto collectible = Cast<ICollectible>(LookAtActor))
		{
			collectible->Collect_Implementation(this);
		}
		else if (auto interactable = Cast<IInteractable>(LookAtActor))
		{
			interactable->Interact_Implementation(this);
		}
	}
}

void AMainCharacter::OnWeaponEquip(const FInputActionValue& Value)
{
	if (!IsBusy())
	{
		if (IsWeaponEquipped())
		{
			ActionState = ECharacterActionState::Animating;
			PlayAnimationMontage(WeaponEquipMontage, FName("Unequip"));
		}
		else
		{
			ActionState = ECharacterActionState::Animating;
			PlayAnimationMontage(WeaponEquipMontage, FName("Equip"));
		}
	}
}

void AMainCharacter::ArmWeapon()
{
	if (RHandEquipped)
	{
		RHandEquipped->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RHandSocket"));
		RHandEquipped->SetActive(true);
		ActiveEquipmentState = ECharacterActiveEquipmentState::RightHandWeapon;
	}
}

void AMainCharacter::DisarmWeapon()
{
	if (RHandEquipped)
	{
		RHandEquipped->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("SwordSlotBack"));
		RHandEquipped->SetActive(false);
		ActiveEquipmentState = ECharacterActiveEquipmentState::Unequipped;
	}
}

void AMainCharacter::OnAttackLight(const FInputActionValue& Value)
{
	if (!IsBusy() && IsWeaponEquipped())
	{
		ActionState = ECharacterActionState::Attacking;
		PlayAnimationMontage(AttackMontage);
	}
}

bool AMainCharacter::IsWeaponEquipped() const
{
	if (ActiveEquipmentState != ECharacterActiveEquipmentState::Unequipped) return true; // TODO: actually to think about shield
	return false;
}

bool AMainCharacter::IsBusy() const
{
	if (ActionState == ECharacterActionState::Unoccupied) return false;
	return true;
}

void AMainCharacter::AttachWeapon(AWeapon* Weapon)
{
	if (!IsWeaponEquipped())
	{
		Weapon->Equip(GetMesh(), this, ECharacterSockets::RHandSocket);
		
		ActiveEquipmentState = ECharacterActiveEquipmentState::RightHandWeapon; // TODO: resolve shield option
		RHandEquipped = Weapon;
	}
}

void AMainCharacter::CollectItem(AItem* Item)
{
	CollectedItems.Add(Item);
}

void AMainCharacter::SetCanLookFor(bool var)
{
	bIsLookingFor = var;
	if (!bIsLookingFor) LookAtActor = nullptr;
}
void AMainCharacter::StartDamaging()
{
	RHandEquipped->SetActive(true);
}

void AMainCharacter::StopDamaging()
{
	RHandEquipped->SetActive(false);
	if (AWeapon* Weapon = Cast<AWeapon>(RHandEquipped))
	{
		Weapon->ResetActorsToIgnore();
	}
}
