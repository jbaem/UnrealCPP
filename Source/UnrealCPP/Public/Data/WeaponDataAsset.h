#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/CommonEnums.h"

#include "WeaponDataAsset.generated.h"

class AWeapon;
class AUsedWeapon;
class APickupWeapon;

UCLASS()
class UNREALCPP_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Data Asset")
	bool IsValid() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Type")
	EItemCode WeaponType = EItemCode::EIC_Basic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Classes")
	TSubclassOf<AWeapon> EquippedWeaponClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Classes")
	TSubclassOf<AUsedWeapon> UsedWeaponClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Classes")
	TSubclassOf<APickupWeapon> PickupWeaponClass = nullptr;
};
