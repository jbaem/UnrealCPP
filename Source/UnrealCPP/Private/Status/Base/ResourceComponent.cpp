#include "Status/Base/ResourceComponent.h"

#include "GameFramework/Actor.h"

UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();
}

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

void UResourceComponent::Heal(float HealthAmount)
{
	SetHealthCurrent(HealthCurrent + HealthAmount);
}

void UResourceComponent::UseStamina(float StaminaCost)
{
	SetStaminaCurrent(StaminaCurrent - StaminaCost);
	
	if (StaminaCurrent <= 0.0f)
	{
		OnStaminaDepleted.Broadcast();
	}
	StartStaminaRegenTimer();
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


inline void UResourceComponent::SetStaminaMaxByStatus(UStatusComponent* InStatus)
{
	StaminaMax = StaminaBase;
	if (InStatus)
	{
		StaminaMax += InStatus->GetAdditionalStamina();
	}
	SetStaminaCurrent(StaminaMax);
}

inline void UResourceComponent::SetHealthCurrent(float NewHealth)
{
	HealthCurrent = FMath::Clamp(NewHealth, 0.0f, HealthMax);
	OnHealthChanged.Broadcast(HealthCurrent, HealthMax);
}

inline void UResourceComponent::SetStaminaCurrent(float NewStamina)
{
	StaminaCurrent = FMath::Clamp(NewStamina, 0.0f, StaminaMax);
	OnStaminaChanged.Broadcast(StaminaCurrent, StaminaMax);
}

void UResourceComponent::StartStaminaRegenTimer()
{
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		ClearAllRegenTimer(TimerManager);
		WaitRegenStaminaCooldown(TimerManager);
	}
}

void UResourceComponent::WaitRegenStaminaCooldown(FTimerManager& TimerManager)
{
	TimerManager.SetTimer(
		StaminaAutoRegenCoolTimerHandle,
		[this, &TimerManager]()
		{
			StartRegenStamina(TimerManager);
		},
		StaminaRegenCoolTime,
		false
	);
}

void UResourceComponent::StartRegenStamina(FTimerManager& TimerManager)
{
	TimerManager.SetTimer(
		StaminaRegenTickTimerHandle,
		this,
		&UResourceComponent::RegenStaminaPerTick,
		StaminaRegenTickInterval, // Tick Interval
		true, // Loop
		0.1f  // First Delay
	);
}

void UResourceComponent::ClearAllRegenTimer(FTimerManager& TimerManager)
{
	TimerManager.ClearTimer(StaminaAutoRegenCoolTimerHandle);
	TimerManager.ClearTimer(StaminaRegenTickTimerHandle);
}

void UResourceComponent::RegenStaminaPerTick()
{
	SetStaminaCurrent(StaminaCurrent + StaminaRegenAmountPerTick);
	StopRegenStaminaFull();
}

void UResourceComponent::StopRegenStaminaFull()
{
	if (StaminaCurrent < StaminaMax) return;
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(StaminaRegenTickTimerHandle);
	}
}
