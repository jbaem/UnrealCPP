#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Common/CommonEnums.h"
#include "Weapon/Data/WeaponDataAsset.h"

#include "WeaponManagerComponent.generated.h"

class AActionCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UWeaponManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AttackEnable(bool Flag);
	void ActivateSlashEffect(bool Flag);
	bool CanAttack();

	void OnAreaAttack();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(EItemCode WeaponCode, int32 Count);
	void EquipNewWeapon(EItemCode WeaponCode, int32 Count);
	bool ShouldDropCurrentWeapon(EItemCode WeaponCode);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropWeapon(EItemCode WeaponCode);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropCurrentWeapon();
	void ResetWeapon();
	void OnAttack();

	AWeapon* GetEquippedWeaponByItemCode(EItemCode ItemCode) const;
	TSubclassOf<AUsedWeapon> GetUsedWeaponClassByItemCode(EItemCode ItemCode) const;
	TSubclassOf<APickupWeapon> GetPickupWeaponClassByItemCode(EItemCode ItemCode) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Weapon|DB")
	TMap<EItemCode, TObjectPtr<UWeaponDataAsset>> WeaponDB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Weapon|Instance")
	TMap<EItemCode, TObjectPtr<AWeapon>> WeaponInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Weapon")
	TObjectPtr<AWeapon> WeaponCurrent = nullptr;

private:
	void InitOwner();
	void InitializeEquippedWeapon();

	void ValidateWeaponDatabase();
	void SpawnWeaponInstances();
	AWeapon* SpawnWeaponInstance(UWeaponDataAsset* InDataAsset);
	void AttachInstanceToOwner(AWeapon* SpawnedWeapon, EItemCode InItemCode);

	UPROPERTY()
	TWeakObjectPtr<AActionCharacter> Owner;
};
