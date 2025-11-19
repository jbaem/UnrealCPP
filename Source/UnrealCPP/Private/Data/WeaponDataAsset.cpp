#include "Data/WeaponDataAsset.h"

#include "Item/WeaponPickUp.h"
#include "Weapon/UsedWeapon.h"
#include "Weapon/Weapon.h"

bool UWeaponDataAsset::IsValid() const
{
	return EquippedWeaponClass != nullptr 
		&& UsedWeaponClass != nullptr
		&& PickupWeaponClass != nullptr;
}