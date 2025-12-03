#include "Framework/System/EnemyCountSubsystem.h"

#include "Framework/System/EnemyCountSettings.h"
#include "Kismet/GameplayStatics.h"

void UEnemyCountSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadEnemyClass();
}

void UEnemyCountSubsystem::RegisterEnemy(AEnemyPawn* Enemy)
{
	if (CanRegisterEnemy(Enemy)) return;
	RegisteredEnemies.Add(Enemy);
	
	GetEnemyCountOnChange();
}

void UEnemyCountSubsystem::UnregisterEnemy(AEnemyPawn* Enemy)
{
	if (CanUnregisterEnemy(Enemy)) return;
	RegisteredEnemies.Remove(Enemy);
	
	if(GetEnemyCountOnChange() <= 0)
	{
		OnAllEnemiesCleared.Broadcast();
	}
}

void UEnemyCountSubsystem::LoadEnemyClass()
{
	const UEnemyCountSettings* settings = GetDefault<UEnemyCountSettings>();
	if (settings && !settings->EnemyClass.IsNull())
	{
		EnemyClass = settings->EnemyClass.LoadSynchronous();
	}
}

bool UEnemyCountSubsystem::CanRegisterEnemy(AEnemyPawn* Enemy)
{
	return !IsValid(Enemy) || RegisteredEnemies.Contains(Enemy);
}

bool UEnemyCountSubsystem::CanUnregisterEnemy(AEnemyPawn*& Enemy)
{
	return !RegisteredEnemies.Contains(Enemy);
}

int32 UEnemyCountSubsystem::GetEnemyCountOnChange()
{
	int32 EnemyCount = GetEnemyCount();
	OnEnemyCountChanged.Broadcast(EnemyCount);
	return EnemyCount;
}
