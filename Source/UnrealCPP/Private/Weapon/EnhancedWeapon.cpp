#include "Weapon/EnhancedWeapon.h"


void AEnhancedWeapon::OnAttack()
{
	UseCountRemain--;
	if (UseCountRemain <= 0)
	{
		OnConsumeDelegate.Broadcast(WeaponID);
	}
}

void AEnhancedWeapon::OnWeaponPickuped()
{
	Super::OnWeaponPickuped();

	UseCountRemain = UseCountMax;
}
