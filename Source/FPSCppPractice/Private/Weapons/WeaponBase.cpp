// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Weapons/BulletBase.h"
#include "TimerManager.h"

// Sets default values
AWeaponBase::AWeaponBase()
	: BulletClass(ABulletBase::StaticClass())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	MuzzlePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
	MuzzlePoint->SetupAttachment(RootComponent);
	MuzzlePoint->ArrowSize = 0.25f;
	MuzzlePoint->ArrowColor = FColor::Cyan;

}

void AWeaponBase::StartFiring_Implementation()
{
	if (!BulletClass)
	{
		//debug text, no bullet attached
		return;
	}

	if (bFullyAutomatic)
	{
		GetWorldTimerManager().SetTimer(AutofireHandle, this, &AWeaponBase::FireWeapon, FireSpeed, true);
		FireWeapon();
	}

	else
		FireWeapon();


}

void AWeaponBase::StopFiring_Implementation()
{
	GetWorldTimerManager().ClearTimer(AutofireHandle);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::FireWeapon()
{
	if (RemainingAmmo > 0)
	{
		RemainingAmmo--;
		//fire
		AActor* BulletOwner = GetOwner() ? GetOwner() : this;
		FTransform SpawnTransform = MuzzlePoint->GetComponentTransform();

		FActorSpawnParameters Params;
		Params.Owner = BulletOwner;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(BulletClass, SpawnTransform, Params);

		OnFireWeapon();
	}
	
}

