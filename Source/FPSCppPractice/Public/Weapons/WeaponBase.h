// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ABulletBase;

UCLASS()
class FPSCPPPRACTICE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MuzzlePoint;
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TSubclassOf<ABulletBase> BulletClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float FireSpeed = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	int RemainingAmmo = 99;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	bool bFullyAutomatic = false;

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")  //BlueprintNativeEvent -> Has function, overrideable in blueprint | BlueprintImplementableEvent -> No function, exposes to bp as event
	void StartFiring();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StopFiring();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnUnequipped();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnFireWeapon();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float Damage = 50;


private:

	UFUNCTION()
	void FireWeapon();

	FTimerHandle AutofireHandle;

};
