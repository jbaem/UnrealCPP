// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Common/CommonEnums.h"

#include "Weapon.generated.h"

class AActionCharacter;

UCLASS()
class UNREALCPP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void PostInitializeComponents() override;

	inline EItemCode GetWeaponID() const { return WeaponID; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void DestroyWeapon();

public:	
	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);
	
	UFUNCTION(BlueprintCallable)
	void StartHoldingTimer();

	// 공격했을 때 실행
	UFUNCTION(BlueprintCallable)
	virtual void OnAttack() {};

	// 이 무기로 공격할 수 있는지 여부 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CanAttack() { return true; }

	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponPickuped(AActionCharacter* Picker);

	virtual int32 GetUsedCountRemain() { return -1; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	EItemCode WeaponID = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration")
	float HoldingDuration = 30.0f;

private:
	TWeakObjectPtr<ACharacter> WeaponOwner = nullptr;

	FTimerHandle HoldingTimerHandle;
};
