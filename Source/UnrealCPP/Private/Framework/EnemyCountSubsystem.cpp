#include "Framework/EnemyCountSubsystem.h"

#include "Settings/EnemyCountSettings.h"
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

void UEnemyCountSubsystem::IncreaseEnemyCount()
{
	EnemyCount++;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Count Increased: %d"), EnemyCount);
	OnIncreaseEnemyCount.Broadcast();
}

void UEnemyCountSubsystem::DecreaseEnemyCount()
{
	EnemyCount--;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Count Decreased: %d"), EnemyCount);
	OnDecreaseEnemyCount.Broadcast();
}
