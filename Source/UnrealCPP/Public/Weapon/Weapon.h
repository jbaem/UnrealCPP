#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Common/CommonEnums.h"

#include "Weapon.generated.h"

class AActionCharacter;

UCLASS()
class UNREALCPP_API AWeapon : public AActor
{
	GENERATED_BODY()
public:	
	AWeapon();

	virtual void PostInitializeComponents() override;

	inline EItemCode GetWeaponID() const { return WeaponID; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual void OnWeaponActivate() {};
	virtual void OnWeaponDeactivate() {};

	void DestroyWeapon();

public:	
	// visability, collision, attack enable/disable
	UFUNCTION(BlueprintCallable)
	void WeaponActivate(bool bActivate);
	
	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);
	
	UFUNCTION(BlueprintCallable)
	void StartHoldingTimer();

	UFUNCTION(BlueprintCallable)
	virtual void OnAttack() {};

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CanAttack() { return true; }

	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponPickuped();

	virtual int32 GetUsedCountRemain() { return -1; }
	virtual void SetUsedCountRemain(int32 Count) { }

	inline void SetWeaponOwner(ACharacter* NewOwner) { WeaponOwner = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	EItemCode WeaponID = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration")
	float HoldingDuration = 30.0f;

private:
	TWeakObjectPtr<ACharacter> WeaponOwner = nullptr;

	FTimerHandle HoldingTimerHandle;
};
