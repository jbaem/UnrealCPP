#include "Weapon/EnhancedWeapon.h"

void AEnhancedWeapon::OnAttack()
{
	UseCountRemain--;
	if (UseCountRemain <= 0)
	{
		OnWeaponUseEnded.Broadcast();
	}
}

void AEnhancedWeapon::OnWeaponPickuped(AActionCharacter* Picker)
{
	Super::OnWeaponPickuped(Picker);

	UseCountRemain = UseCountMax;
}
