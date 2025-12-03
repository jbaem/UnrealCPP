#include "Weapon/Data/WeaponDataAsset.h"

#include "Weapon/Extra/UsedWeapon.h"
#include "Weapon/Base/Weapon.h"
#include "Weapon/Extra/PickupWeapon.h"

bool UWeaponDataAsset::IsValid() const
{
	return EquippedWeaponClass != nullptr 
		&& UsedWeaponClass != nullptr
		&& PickupWeaponClass != nullptr;
}