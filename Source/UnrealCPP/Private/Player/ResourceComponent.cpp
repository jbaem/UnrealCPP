// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ResourceComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UResourceComponent::TakeDamage(float HealthAmount)
{
	SetHealthCurrent(HealthCurrent - HealthAmount);

	if (!IsAlive())
	{
		OnDie.Broadcast();
	}
}

void UResourceComponent::RestoreHealth(float HealthAmount)
{
	SetHealthCurrent(HealthCurrent + HealthAmount);
}

void UResourceComponent::UseStamina(float StaminaCost)
{
	SetStaminaCurrent(StaminaCurrent - StaminaCost);
	
	if (StaminaCurrent <= 0.0f)
	{
		// alert zero stamina to using delegate
		OnStaminaDepleted.Broadcast();
	}
	StartStaminaRegenTimer();
}

void UResourceComponent::StartStaminaRegenTimer()
{
	UWorld* world = GetWorld();
	if (!world) return;

	FTimerManager& timerManager = world->GetTimerManager();

	timerManager.ClearTimer(StaminaAutoRegenCoolTimerHandle);
	timerManager.ClearTimer(StaminaRegenTickTimerHandle);

	timerManager.SetTimer(
		StaminaAutoRegenCoolTimerHandle,
		[this, &timerManager]() {
			timerManager.SetTimer(
				StaminaRegenTickTimerHandle,
				this,
				&UResourceComponent::RegenStaminaPerTick,
				StaminaRegenTickInterval, // Tick Interval
				true, // Loop
				0.1f  // First Delay
			);
		},
		StaminaRegenCoolTime,
		false
	);
}

void UResourceComponent::RegenStaminaPerTick()
{
	SetStaminaCurrent(StaminaCurrent + StaminaRegenAmountPerTick);

	if (StaminaCurrent >= StaminaMax)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			world->GetTimerManager().ClearTimer(StaminaRegenTickTimerHandle);
		}
	}
}

void UResourceComponent::SetAllResourceByStatus(UStatusComponent* InStatus)
{
	SetHealthMaxByStatus(InStatus);
	SetStaminaMaxByStatus(InStatus);
}

inline void UResourceComponent::SetHealthMaxByStatus(UStatusComponent* InStatus)
{
	HealthMax = HealthBase;
	if (InStatus)
	{
		HealthMax += InStatus->GetAdditionalHealth();
	}
	SetHealthCurrent(HealthMax);
}

inline void UResourceComponent::SetHealthCurrent(float NewHealth)
{
	HealthCurrent = FMath::Clamp(NewHealth, 0.0f, HealthMax);
	OnHealthChanged.Broadcast(HealthCurrent, HealthMax);
}

inline void UResourceComponent::SetStaminaMaxByStatus(UStatusComponent* InStatus)
{
	StaminaMax = StaminaBase;
	if (InStatus)
	{
		StaminaMax += InStatus->GetAdditionalStamina();
	}
	SetStaminaCurrent(StaminaMax);
}

inline void UResourceComponent::SetStaminaCurrent(float NewStamina)
{
	StaminaCurrent = FMath::Clamp(NewStamina, 0.0f, StaminaMax);
	OnStaminaChanged.Broadcast(StaminaCurrent, StaminaMax);
}
