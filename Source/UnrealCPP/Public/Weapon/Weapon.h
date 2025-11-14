// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

#include "Weapon.generated.h"

UCLASS()
class UNREALCPP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:	
	UFUNCTION(BlueprintCallable)
	inline void SetWeaponOwner(ACharacter* NewOwner) { WeaponOwner = NewOwner; }

	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UDamageType> DamageType = nullptr;


private:
	TWeakObjectPtr<ACharacter> WeaponOwner = nullptr;

};
