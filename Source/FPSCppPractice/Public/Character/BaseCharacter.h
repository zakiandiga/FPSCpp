// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCameraComponent; //forward declare

UCLASS()
class FPSCPPPRACTICE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sprint|Character", meta=(AllowPrivateAccess="true"))
	UCameraComponent* Camera; //pointer to the component

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sprint|Character", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* HandMesh;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UCameraComponent* GetCameraComponent() { return Camera; } //getter
	USkeletalMeshComponent* GetMesh1P() { return HandMesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float delta);
	void MoveRight(float delta);
	void MoveUp(float delta);
	void Turn(float delta);
	void TurnRate(float delta);
	void LookUp(float delta);
	void LookUpRate(float delta);

	void StartJump();
	void StopJump();
	void StartShoot();
	void StopShoot();
	void StartSprint();
	void StopSprint();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CycleWeapon(bool bForwards);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void GrantWeapon(TSubclassOf<AWeaponBase> NewWeapon, bool bEquip);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RemoveWeapon(TSubclassOf<AWeaponBase> WeaponToRemove);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
		TArray<TSubclassOf<AWeaponBase>> WeaponInventory;

private:

	AWeaponBase* EquippedWeapon;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	FName AttachSocket;

	UFUNCTION(BlueprintPure, Category = "Weapons")
	AWeaponBase* GetEquippedWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	AWeaponBase* EquipWeapon(TSubclassOf<AWeaponBase> NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void UnequipWeapon();
		


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementProperty")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementProperty")
	float BaseLookAtRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementProperty")
	float MaxSprintSpeed = 900.0f;

private:
	float DefaultWalkSpeed;
};
