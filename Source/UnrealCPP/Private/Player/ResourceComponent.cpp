// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ResourceComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	StaminaCurrent = StaminaMax;
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UResourceComponent::TakeDamage(float HealthAmount)
{
	HealthCurrent = FMath::Clamp(HealthCurrent - HealthAmount, 0.0f, HealthMax);
	OnHealthChanged.Broadcast(HealthCurrent, HealthMax);

	if (!IsAlive())
	{
		OnDie.Broadcast();
	}
}

void UResourceComponent::RestoreHealth(float HealthAmount)
{
	HealthCurrent = FMath::Clamp(HealthCurrent + HealthAmount, 0.0f, HealthMax);
	OnHealthChanged.Broadcast(HealthCurrent, HealthMax);
}

void UResourceComponent::UseStamina(float StaminaCost)
{
	StaminaCurrent = FMath::Clamp(StaminaCurrent - StaminaCost, 0.0f, StaminaMax);
	OnStaminaChanged.Broadcast(StaminaCurrent, StaminaMax);
	
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
	StaminaCurrent += StaminaRegenAmountPerTick;
	StaminaCurrent = FMath::Clamp(StaminaCurrent, 0.0f, StaminaMax);
	OnStaminaChanged.Broadcast(StaminaCurrent, StaminaMax);
	
	if (StaminaCurrent >= StaminaMax)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			world->GetTimerManager().ClearTimer(StaminaRegenTickTimerHandle);
		}
	}
}

