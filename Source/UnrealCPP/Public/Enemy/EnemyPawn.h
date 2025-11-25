#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "EnemyPawn.generated.h"

UCLASS()
class UNREALCPP_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	AEnemyPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(
		float Damage,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;
	
	void SetDamage(float DamageAmount);

	FORCEINLINE bool IsDamageWidgetActive() const { return bIsDamageWidgetActive; }
	void ActivateDamageWidget(float DamageAmount);
	void DeactivateDamageWidget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> DamageWidgetComponent;

private:
	FTimerHandle DeactivateDamageWidgetTimerHandle;
	
	bool bIsDamageWidgetActive = false;
};
