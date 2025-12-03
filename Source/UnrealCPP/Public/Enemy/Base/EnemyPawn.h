#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/Common/CommonStructs.h"

#include "EnemyPawn.generated.h"

UCLASS()
class UNREALCPP_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	AEnemyPawn();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Components")
	TObjectPtr<class UResourceComponent> ResourceComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Components")
	TObjectPtr<USceneComponent> PopupLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Damage Popup")
	TSubclassOf<class ADamagePopup> DamagePopupClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Drop Item")
	TObjectPtr<class UDataTable> DropItemTable = nullptr;

private:
	void InitComponents();
	void InitMeshComponent();
	void InitResourceComponent();
	void InitPopupLocation();

	void BindOnTakeDamage();
	void RegisterEnemy();
	void UnregisterEnemy();

	bool CanDamaged(float Damage);
	void ShowDamagePopup(float Damage);
	void TakeDamageProcess(float Damage);

	void StartInvincible(float Damage);
	void StartInvincibleTimer();
	void SetIsInvincible(float Damage);

	void OnDie();
	void DropItem();
	void SpawnPickupItem(EItemCode ItemCode, float DropRate);
	FVector GetRandomDropVelocity();

	FTimerHandle InvincibleTimerHandle;

	bool bIsInvincible = false;
	float LastDamage = 0.0f;
};
