// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/WeaponBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));   //spawning CameraComponent with name Camera outside the constructor
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));

	Camera->SetupAttachment(GetRootComponent()); //Attach this component to the root
	Camera->bUsePawnControlRotation = true;
	HandMesh->SetupAttachment(Camera);
	HandMesh->SetOnlyOwnerSee(true);
	HandMesh->SetOwnerNoSee(false);
	HandMesh->SetCastShadow(false);
	HandMesh->SetReceivesDecals(false);

	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;	
}

void ABaseCharacter::Destroyed()
{
	UnequipWeapon();

	Super::Destroyed();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) //default function for character
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward); //string of axis name, object, function
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &ABaseCharacter::MoveUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &ABaseCharacter::TurnRate);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ABaseCharacter::LookUpRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABaseCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ABaseCharacter::StopJump);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ABaseCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ABaseCharacter::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABaseCharacter::StartShoot);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABaseCharacter::StopShoot);
}

void ABaseCharacter::MoveForward(float delta)
{
	if (delta != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), delta);
	}
	else
		StopSprint();
}

void ABaseCharacter::MoveRight(float delta)
{
	if (delta != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), delta);
	}
}

void ABaseCharacter::MoveUp(float delta)
{
	if (delta != 0.0f)
	{
		AddMovementInput(GetActorUpVector(), delta);
	}
}

void ABaseCharacter::Turn(float delta)
{
	AddControllerYawInput(delta);
}

void ABaseCharacter::TurnRate(float delta)
{
	AddControllerYawInput(delta * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUp(float delta)
{
	AddControllerPitchInput(delta);
}

void ABaseCharacter::LookUpRate(float delta)
{
	AddControllerPitchInput(delta * BaseLookAtRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::StartJump()
{
	Jump();
}

void ABaseCharacter::StopJump()
{
	StopJumping();
}

void ABaseCharacter::StartShoot()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartFiring();
	}

	else
	{
		//No weapon equipped
	}
}

void ABaseCharacter::StopShoot()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StopFiring();
	}

}

void ABaseCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
}

void ABaseCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void ABaseCharacter::CycleWeapon(bool bForwards)
{
	if (WeaponInventory.Num() == 0) //if no weapon in the inventory
	{
		return;
	}

	int CurrentEquippedID = WeaponInventory.Find(EquippedWeapon->GetClass());

	if (CurrentEquippedID == INDEX_NONE)
	{
		CurrentEquippedID = 0;
	}

	int DesiredID;

	int y = WeaponInventory.Num();

	if (bForwards) //Cycle forward through the array
	{
		int x = CurrentEquippedID + 1;
		FMath::Fmod(x, y);
	}
	else //Cycle backward through the array
	{
		int x = CurrentEquippedID - 1;

		DesiredID = ((x % y) + y) % y;
	}

	if (!WeaponInventory[DesiredID]->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon not found in $s"), *this->GetName());
		return;
	}

	EquipWeapon(WeaponInventory[DesiredID]);
}

void ABaseCharacter::GrantWeapon(TSubclassOf<AWeaponBase> NewWeapon, bool bEquip)
{
	WeaponInventory.AddUnique(NewWeapon);

	if (bEquip)
	{
		EquipWeapon(NewWeapon);
	}
}

void ABaseCharacter::RemoveWeapon(TSubclassOf<AWeaponBase> WeaponToRemove)
{
	if (GetEquippedWeapon()->IsA(WeaponToRemove))
	{
		UnequipWeapon();
	}

	WeaponInventory.Remove(WeaponToRemove);
}

AWeaponBase* ABaseCharacter::GetEquippedWeapon()
{
	return EquippedWeapon;
}

AWeaponBase* ABaseCharacter::EquipWeapon(TSubclassOf<AWeaponBase> NewWeapon)
{
	if (EquippedWeapon)
	{
		UnequipWeapon();
	}
	
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(NewWeapon, Params);

	if (EquippedWeapon)
	{
		if (UGameplayStatics::GetPlayerCharacter(this, 0) == this)
		{
			EquippedWeapon->AttachToComponent(HandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
		}
		else
		{
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
		}

		EquippedWeapon->OnEquipped();
		return EquippedWeapon;
	}


	return nullptr;
}

void ABaseCharacter::UnequipWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->OnUnequipped();
	}
}

