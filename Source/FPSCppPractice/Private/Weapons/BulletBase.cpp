// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BulletBase.h"
#include "Weapons/WeaponBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	SetRootComponent(HitSphere);

	HitSphere->InitSphereRadius(12.0f);
	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	HitSphere->SetNotifyRigidBodyCollision(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(HitSphere);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (GetOwner())
	{
		MeshComp->IgnoreActorWhenMoving(GetOwner(), true);
	}

	if (GetOwner() && GetOwner()->GetOwner())
	{
		MeshComp->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
	}

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->InitialSpeed = 3000.0f;
	ProjectileMovementComp->Velocity = FVector(1, 0, 0);


}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	HitSphere->OnComponentHit.AddDynamic(this, &ABulletBase::OnHit);
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
	{
		return; //In case OtherActor is null, don't run this function
	}
	
	AWeaponBase* MyWeapon = Cast<AWeaponBase>(GetOwner());
	float DamageToDeal;

	if (MyWeapon)
	{
		DamageToDeal = MyWeapon->Damage;
	}
	else
	{
		DamageToDeal = 50;
	}

	float DamageMultiplier = 1.0f;
	if (Hit.BoneName == TEXT("Head"))
	{
		DamageMultiplier = 2.0f;
	}

	float TotalDamage = DamageToDeal * DamageMultiplier;
	
	UGameplayStatics::ApplyDamage(OtherActor, TotalDamage, nullptr, GetOwner(), UDamageType::StaticClass());

	OnImpact(Hit);

}

