#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Common/CommonStructs.h"

#include "TestEnemyDamage.generated.h"

UCLASS()
class UNREALCPP_API ATestEnemyDamage : public APawn
{
	GENERATED_BODY()

public:
	ATestEnemyDamage();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	void OnDie();
	void DropItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UResourceComponent> ResourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> PopupLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ADamagePopupActor> DamagePopupClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	TObjectPtr<class UDataTable> DropItemTable = nullptr;

private:
	FTimerHandle InvincibleTimerHandle;

	bool bInvincible = false;
	float LastDamage = 0.0f;
};
