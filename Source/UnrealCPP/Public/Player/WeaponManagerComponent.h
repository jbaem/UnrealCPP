#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnums.h"
#include "Data/WeaponDataAsset.h"

#include "WeaponManagerComponent.generated.h"

class AActionCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UWeaponManagerComponent();

	AWeapon* GetEquippedWeaponByItemCode(EItemCode ItemCode) const;
	TSubclassOf<AUsedWeapon> GetUsedWeaponClassByItemCode(EItemCode ItemCode) const;
	TSubclassOf<AWeaponPickUp> GetPickupWeaponClassByItemCode(EItemCode ItemCode) const;
protected:
	virtual void BeginPlay() override;

private:
	void ValidateWeaponDatabase();
	void SpawnWeaponInstances();

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Database")
	TMap<EItemCode, TObjectPtr<UWeaponDataAsset>> WeaponDatabase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Instance")
	TMap<EItemCode, TObjectPtr<AWeapon>> WeaponInstances;
private:
	UPROPERTY()
	TWeakObjectPtr<AActionCharacter> OwnerPlayer;
};
