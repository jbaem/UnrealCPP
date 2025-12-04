#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Core/Common/CommonEnums.h"

#include "Weapon.generated.h"

class AActionCharacter;

UCLASS()
class UNREALCPP_API AWeapon : public AActor
{
	GENERATED_BODY()
public:	
	AWeapon();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void WeaponActivate(bool bActivate);
	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);
	UFUNCTION(BlueprintCallable)
	void ActivateSlashEffect(bool bAtivate);
	UFUNCTION(BlueprintCallable)
	void StartHoldingTimer();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnAttack() {};
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CanAttack() { return true; }
	
	UFUNCTION(BlueprintCallable)
	virtual void DamageToTarget(AActor* Target);
	UFUNCTION(BlueprintCallable)
	virtual void DamageToArea();
	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponPickuped();

	virtual int32 GetUsedCountRemain() { return -1; }
	virtual void SetUsedCountRemain(int32 Count) { }
	inline EItemCode GetWeaponID() const { return WeaponID; }
	inline void SetWeaponOwner(ACharacter* NewOwner) { WeaponOwner = NewOwner; }

protected:
	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual void OnWeaponActivate() {};
	virtual void OnWeaponDeactivate() {};

	virtual void DestroyWeapon() {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> WeaponEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraSystem> AreaAttackEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	EItemCode WeaponID = EItemCode::EIC_None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UDamageType> DamageType = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float AreaInnerRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float AreaOuterRadius = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float Falloff = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Area")
	float DebugDurtaion = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration")
	float HoldingDuration = 30.0f;

private:
	void InitRoot();
	void InitMeshComponent();
	void InitCapsuleCollision();
	void InitVFX();



	TWeakObjectPtr<ACharacter> WeaponOwner = nullptr;

	FTimerHandle HoldingTimerHandle;
};
