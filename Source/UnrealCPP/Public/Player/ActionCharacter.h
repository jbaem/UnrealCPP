#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "AnimNotify/AnimNotifyState_AttackTrace.h"
#include "InputActionValue.h"
#include "Weapon/Weapon.h"
#include "InventoryOwner.h"
#include "Weapon/UsedWeapon.h"
#include "Item/WeaponPickUp.h"

#include "ActionCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UResourceComponent;
class UStatusComponent;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter, public IInventoryOwner
{
	GENERATED_BODY()
public:
	AActionCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddItem_Implementation(EItemCode Code);

	inline void SetSectionJumpNotify(class UAnimNotifyState_SectionJump* InNotify)
	{
		SectionJumpNotify = InNotify;
		bComboReady = SectionJumpNotify != nullptr;
	}

	inline void SetAttackTraceNotify(class UAnimNotifyState_AttackTrace* InNotify);

	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void TestDropUsedWeapon();
	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void TestDropCurrentWeapon();

protected:
	void OnMoveInput(const FInputActionValue& Value);
	void OnRollInput(const FInputActionValue& Value);
	void OnAttack1Input(const FInputActionValue& Value);
	void OnAttack2Input(const FInputActionValue& Value);

	void SetSprintMode();
	void SetWalkMode();

	void SpendSprintStamina(float DeltaTime);

	void PlayAttack1();
	void PlayComboAttack1();

	void PlayAttack2();
	void PlayComboAttack2();

	void EquipWeapon();
	void DropUsedWeapon();
	void DropCurrentWeapon();

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UFUNCTION()
	void OnStaminaDepleted();

public:
	UResourceComponent* GetResourceComponent() const { return ResourceComponent; }
	UStatusComponent* GetStatusComponent() const { return StatusComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	TObjectPtr<UResourceComponent> ResourceComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Status")
	TObjectPtr<UStatusComponent> StatusComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TObjectPtr<USceneComponent> DropLocation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TSubclassOf<AWeapon> DefaultWeaponClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TObjectPtr<AWeapon> PlayerWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TMap<EItemCode, TSubclassOf<AUsedWeapon>> UsedWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TMap<EItemCode, TSubclassOf<AWeaponPickUp>> PickupWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Roll = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Walk")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Sprint")
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Sprint")
	float SprintStaminaCost = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Roll")
	float RollStaminaCost = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Attack")
	float AttackStaminaCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Attack")
	float Attack2StaminaCost = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Animation|Montage")
	TObjectPtr<UAnimMontage> RollMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Animation|Montage")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Animation|Montage")
	TObjectPtr<UAnimMontage> Attack2Montage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprinting = false;

private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> ActionAnimInstance = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_AttackTrace> AttackTraceNotify = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	//bool bWeaponUseEnded = false;

	bool bComboReady = false;
};
