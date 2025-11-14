// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();
	
public:
	inline int32 GetStrength() const { return Strength; }
	inline int32 GetAgility() const { return Agility; }
	inline int32 GetVitality() const { return Vitality; }
	float GetAttackPower();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status", meta = (ClampMin = "0", ClampMax = "20"))
	int32 Strength = 6;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status", meta = (ClampMin = "0", ClampMax = "20"))
	int32 Agility = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status", meta = (ClampMin = "0", ClampMax = "20"))
	int32 Vitality = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifires")
	float AttackModifier = 1.0f;
};
