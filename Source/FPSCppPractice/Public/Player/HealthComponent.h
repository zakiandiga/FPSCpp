// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UDamageType;
class AController;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDied);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCPPPRACTICE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	int CurrentHealth;

	UFUNCTION()
	void OnOwnerTakenDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, AController* InstrigatedBy, AActor* DamageCauser);

public:	
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int StartingHealth;

	UFUNCTION(BlueprintPure, Category = "Health")
	int GetCurrentHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
	bool GetIsDead();

	UPROPERTY(BlueprintAssignable, Category = "Health") //BlueprintAssignable -> assign to listen to the event in blueprint | BlueprintCallable -> fire the event in blueprint
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDied OnDied;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
