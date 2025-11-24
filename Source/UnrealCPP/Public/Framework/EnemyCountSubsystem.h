#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "EnemyCountSubsystem.generated.h"

class ATestEnemyDamage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncreaseEnemyCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDecreaseEnemyCount);

UCLASS()
class UNREALCPP_API UEnemyCountSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Count")
	void IncreaseEnemyCount();
	UFUNCTION(BlueprintCallable, Category = "Enemy Count")
	void DecreaseEnemyCount();

protected:
	UPROPERTY()
	TSubclassOf<ATestEnemyDamage> EnemyClass;

	UPROPERTY()
	FOnIncreaseEnemyCount OnIncreaseEnemyCount;
	UPROPERTY()
	FOnDecreaseEnemyCount OnDecreaseEnemyCount;

private:
	uint32 EnemyCount = 0;
};
