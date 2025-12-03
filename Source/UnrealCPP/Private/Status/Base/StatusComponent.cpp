#include "Status/Base/StatusComponent.h"

UStatusComponent::UStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UStatusComponent::GetAttackPower()
{
	return Strength * AttackModifier;
}

float UStatusComponent::GetAdditionalHealth()
{
	return (Strength * HealthStrengthModifier + Agility * HealthAgilityModifier) * Vitality;
}

float UStatusComponent::GetAdditionalStamina()
{
	return (Strength * StaminaStrengthModifier + Agility * StaminaAgilityModifier) * Vitality;
}

