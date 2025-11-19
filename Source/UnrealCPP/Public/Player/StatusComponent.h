#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StatusComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UStatusComponent();
	
public:
	float GetAttackPower();
	float GetAdditionalHealth();
	float GetAdditionalStamina();

	inline int32 GetStrength() const { return Strength; }
	inline int32 GetAgility() const { return Agility; }
	inline int32 GetVitality() const { return Vitality; }

protected:
	// Primitive
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Primitive", meta = (ClampMin = "0", ClampMax = "20"))
	int32 Strength = 6;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Primitive", meta = (ClampMin = "0", ClampMax = "20"))
	int32 Agility = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Primitive", meta = (ClampMin = "0", ClampMax = "20"))
	int32 Vitality = 10;

	// Modifier - Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Modifier|Health")
	float HealthStrengthModifier = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Modifier|Health")
	float HealthAgilityModifier = 0.2f;

	// Modifier - Stamina
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Modifier|Stamina")
	float StaminaStrengthModifier = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Modifier|Stamina")
	float StaminaAgilityModifier = 0.4f;

	// Modifier - Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Status|Modifier|Attack")
	float AttackModifier = 1.0f;
};
