// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ResourceComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	StaminaCurrent = StaminaMax;
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResourceComponent::UseStamina(float StaminaCost)
{
	StaminaCurrent -= StaminaCost;
	if (StaminaCurrent <= 0.0f)
	{
		StaminaCurrent = 0.0f;
		// alert zero stamina
		// TODO: SetWalkMode();
	}
	StartStaminaRegenTimer();
}

void UResourceComponent::StartStaminaRegenTimer()
{
	UWorld* world = GetWorld();
	if (!world) return;

	FTimerManager& timerManager = world->GetTimerManager();

	//CoolTime 이 지나면 bUseStamina 를 true 로 변경
	timerManager.ClearTimer(StaminaAutoRegenCoolTimerHandle);
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
	if (StaminaCurrent >= StaminaMax)
	{
		StaminaCurrent = StaminaMax;

		UWorld* world = GetWorld();
		if (world)
		{
			world->GetTimerManager().ClearTimer(StaminaRegenTickTimerHandle);
		}
	}
}

