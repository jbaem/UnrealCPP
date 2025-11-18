#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.h"

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
	UResourceComponent();

protected:
	virtual void BeginPlay() override;

public:	
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

protected:
	void StartStaminaRegenTimer();
	void RegenStaminaPerTick();

public:
	inline float GetHealthCurrent() const { return HealthCurrent; }
	inline float GetHealthMax() const { return HealthMax; }
	inline float GetStaminaCurrent() const { return StaminaCurrent; }
	inline float GetStaminaMax() const { return StaminaMax; }

	void SetAllResourceByStatus(UStatusComponent* InStatus);
	inline void SetHealthMaxByStatus(UStatusComponent* InStatus);
	inline void SetHealthCurrent(float NewHealth);
	inline void SetStaminaMaxByStatus(UStatusComponent* InStatus);
	inline void SetStaminaCurrent(float NewStamina);

public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnDie OnDie;

	//non-dynamic delegate
	//UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStaminaDepleted OnStaminaDepleted;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStaminaChanged OnStaminaChanged;
protected:
	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthBase = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthMax = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthCurrent = 100.0f;

	// Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaBase = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaMax = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaCurrent = 100.0f;

	// Stamina Regen
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
