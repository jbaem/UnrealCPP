#include "Data/WeaponDataAsset.h"

#include "Weapon/UsedWeapon.h"
#include "Weapon/Weapon.h"
#include "Item/PickupWeapon.h"

bool UWeaponDataAsset::IsValid() const
{
	return EquippedWeaponClass != nullptr 
		&& UsedWeaponClass != nullptr
		&& PickupWeaponClass != nullptr;
}