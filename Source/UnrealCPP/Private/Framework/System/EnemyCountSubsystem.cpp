#include "Framework/System/EnemyCountSubsystem.h"

#include "Framework/System/EnemyCountSettings.h"
#include "Kismet/GameplayStatics.h"

void UEnemyCountSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UEnemyCountSettings* settings = GetDefault<UEnemyCountSettings>();
	if(settings && !settings->DamagePopupClass.IsNull())
	{
		EnemyClass = settings->DamagePopupClass.LoadSynchronous();
	}
}

void UEnemyCountSubsystem::RegisterEnemy(ATestEnemyDamage* Enemy)
{
	if(!IsValid(Enemy) || RegisteredEnemies.Contains(Enemy))
	{
		return;
	}

	RegisteredEnemies.Add(Enemy);
	int32 EnemyCount = RegisteredEnemies.Num();
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Count Increased: %d"), EnemyCount);
	OnEnemyCountChanged.Broadcast(EnemyCount);
}

void UEnemyCountSubsystem::UnregisterEnemy(ATestEnemyDamage* Enemy)
{
	if(!RegisteredEnemies.Contains(Enemy))
	{
		return;
	}

	RegisteredEnemies.Remove(Enemy);
	int32 EnemyCount = RegisteredEnemies.Num();

	OnEnemyCountChanged.Broadcast(EnemyCount);
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Count Decreased: %d"), EnemyCount);
	if(EnemyCount <= 0)
	{
		OnAllEnemiesCleared.Broadcast();
		//UE_LOG(LogTemp, Warning, TEXT("All Enemies Cleared!"));
	}
}
