#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "EnemyCountSubsystem.generated.h"

class ATestEnemyDamage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChanged, int32, NewCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesCleared);

UCLASS()
class UNREALCPP_API UEnemyCountSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Count")
	void RegisterEnemy(ATestEnemyDamage* Enemy);
	UFUNCTION(BlueprintCallable, Category = "Enemy Count")
	void UnregisterEnemy(ATestEnemyDamage* Enemy);

	UFUNCTION(BlueprintCallable, Category = "Enemy Count")
	int32 GetEnemyCount() const { return RegisteredEnemies.Num(); }

protected:
	UPROPERTY()
	TSubclassOf<ATestEnemyDamage> EnemyClass;

	UPROPERTY(BlueprintAssignable, Category = "Enemy Count")
	FOnEnemyCountChanged OnEnemyCountChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Enemy Count")
	FOnAllEnemiesCleared OnAllEnemiesCleared;

private:
	UPROPERTY(VisibleAnywhere, Category = "Enemy Count")
	TSet<TObjectPtr<ATestEnemyDamage>> RegisteredEnemies;
};
