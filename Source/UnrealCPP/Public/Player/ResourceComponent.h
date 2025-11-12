// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float); // Non-dynamic delegate for comparing 

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, Current, float, Max);

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	void TakeDamage(float HealthAmount);

	UFUNCTION(BlueprintCallable)
	void RestoreHealth(float HealthAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool IsAlive() const { return HealthCurrent > 0.0f; };

	UFUNCTION(BlueprintCallable)
	void UseStamina(float StaminaCost);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool HasEnoughStamina(float StaminaCost) const { return StaminaCurrent >= StaminaCost; };

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnDie OnDie;

	//non-dynamic delegate
	//UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStaminaDepleted OnStaminaDepleted;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStaminaChanged OnStaminaChanged;

	// Getters
	inline float GetHealthCurrent() const { return HealthCurrent; }
	inline float GetHealthMax() const { return HealthMax; }
	inline float GetStaminaCurrent() const { return StaminaCurrent; }
	inline float GetStaminaMax() const { return StaminaMax; }

	// Setters
	inline void SetHealthCurrent(float NewHealth)
	{
		HealthCurrent = FMath::Clamp(NewHealth, 0.0f, HealthMax); 
		OnHealthChanged.Broadcast(HealthCurrent, HealthMax);
	}
	inline void SetStaminaCurrent(float NewStamina) 
	{ 
		StaminaCurrent = FMath::Clamp(NewStamina, 0.0f, StaminaMax);
		OnStaminaChanged.Broadcast(StaminaCurrent, StaminaMax);
	}

protected:	
	void StartStaminaRegenTimer();
	void RegenStaminaPerTick();

protected:
	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthMax = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthCurrent = 100.0f;

	// Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaMax = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaCurrent = 100.0f;

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
