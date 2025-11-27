// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Pickup.h"
#include "PickupWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API APickupWeapon : public APickup
{
	GENERATED_BODY()
	
public:
	virtual void OnPickupComplete_Implementation() override;

	void SetAttackCountRemain(int32 Count) { AttackCountRemain = Count; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponCode WeaponCode = EWeaponCode::EWC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 AttackCountRemain = 3;
};
