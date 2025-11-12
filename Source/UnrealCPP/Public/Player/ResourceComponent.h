// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ResourceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void UseStamina(float StaminaCost);

	UFUNCTION(BlueprintCallable, Blueprintpure)
	inline bool HasEnoughStamina(float StaminaCost) const { return StaminaCurrent >= StaminaCost; };

	inline float GetStaminaCurrent() const { return StaminaCurrent; }

protected:	
	void StartStaminaRegenTimer();
	void RegenStaminaPerTick();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaMax = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenAmountPerTick = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRegenCoolTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRegenTickInterval = 1.0f;

private:
	FTimerHandle StaminaAutoRegenCoolTimerHandle;
	FTimerHandle StaminaRegenTickTimerHandle;
};
