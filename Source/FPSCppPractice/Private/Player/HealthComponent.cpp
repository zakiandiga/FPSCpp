// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealthComponent.h"
#include "GameFramework/Actor.h"

void UHealthComponent::OnOwnerTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstrigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= (int)Damage;
	CurrentHealth = FMath::Clamp<int>(CurrentHealth, 0, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth);

	if (GetIsDead())
	{
		OnDied.Broadcast();
	}
}

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


int UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

bool UHealthComponent::GetIsDead()
{
	return CurrentHealth <= 0;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	StartingHealth = StartingHealth == 0 ? MaxHealth : StartingHealth;
	CurrentHealth = StartingHealth;
	
	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnOwnerTakenDamage); //subscribe to AActor::OnTakeAnyDamage
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

