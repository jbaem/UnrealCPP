#include "Weapon/EnhancedWeapon.h"


void AEnhancedWeapon::BeginPlay()
{
	Super::BeginPlay();

	UseCountRemain = UseCountMax;
}

void AEnhancedWeapon::OnAttack()
{
	UseCountRemain--;
	//UE_LOG(LogTemp, Log, TEXT("Enhanced Weapon Used. Remaining Uses: %d"), UseCountRemain);
	if (UseCountRemain <= 0)
	{
		OnConsumeDelegate.Broadcast(WeaponID);
		//UE_LOG(LogTemp, Log, TEXT("Enhanced Weapon Consumed."));
	}
}

void AEnhancedWeapon::OnWeaponPickuped()
{
	Super::OnWeaponPickuped();

	UseCountRemain = UseCountMax;
}

void AEnhancedWeapon::OnWeaponActivate()
{
	UseCountRemain = UseCountMax;
}
