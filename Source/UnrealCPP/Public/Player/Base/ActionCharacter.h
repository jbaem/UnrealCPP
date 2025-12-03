#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "InputActionValue.h"
#include "Core/Interface/InventoryOwner.h"
#include "Combat/AnimNotify/AnimNotifyState_SectionJump.h"
#include "Combat/AnimNotify/AnimNotifyState_AttackTrace.h"
#include "Weapon/Base/Weapon.h"
#include "Weapon/Extra/UsedWeapon.h"
#include "Item/Pickupable/Pickup.h"

#include "ActionCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UResourceComponent;
class UStatusComponent;
class UInventoryComponent;
class UEnhancedInputComponent;

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

public:
	// IInventoryOwner interface
	virtual void AddItem_Implementation(UItemDataAsset* ItemData, int32 Count);
	virtual void AddWeapon_Implementation(EItemCode Code, int32 AttackCount);
	virtual void AddMoney_Implementation(int32 Amount);
	virtual void UseMoney_Implementation(int32 Amount);
	// notify setters
	inline void SetSectionJumpNotify(class UAnimNotifyState_SectionJump* InNotify);
	inline void SetAttackTraceNotify(class UAnimNotifyState_AttackTrace* InNotify);
	inline void SetSlashEffectNotify(class UAnimNotifyState_SlashEffect* InNotify);
	void OnAreaAttack();
	// weapon management
	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void EquipWeapon(EItemCode WeaponCode, int32 Count);
	void EquipNewWeapon(EItemCode WeaponCode, int32 Count);
	bool ShouldDropCurrentWeapon(EItemCode WeaponCode);
	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void DropWeapon(EItemCode WeaponCode);
	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void DropCurrentWeapon();

protected:
	// Input handlers
	void OnMoveInput(const FInputActionValue& Value);
	bool CanMove();
	void OnRollInput(const FInputActionValue& Value);
	bool CanRoll();
	void OnAttack1Input(const FInputActionValue& Value);
	bool CanAttack1();
	void OnAttack2Input(const FInputActionValue& Value);

	bool CanAttack2();

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UResourceComponent* GetResourceComponent() const { return ResourceComponent; }
	UStatusComponent* GetStatusComponent() const { return StatusComponent; }
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	// Components
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
	// DataAssets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UPlayerInputData> InputData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<class UPlayerMontageData> MontageData;
	// Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Walk")
	float WalkSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Sprint")
	float SprintSpeed = 1200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Sprint")
	float SprintStaminaCost = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Roll")
	float RollStaminaCost = 40.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Combat|Attack")
	float AttackStaminaCost = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Combat|Attack")
	float Attack2StaminaCost = 15.0f;
	// Managers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TObjectPtr<class UWeaponManagerComponent> WeaponManager = nullptr;
	// Current Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TObjectPtr<AWeapon> PlayerWeapon = nullptr;
	// States
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprinting = false;

private:
	// Init constructor settings
	void InitCameraSystem();
	void InitComponents();
	void InitDropLocation();
	void InitMovementRotation();
	void InitSpringArm();
	void InitCamera();
	// Init begin play settings
	void InitResourceByStatus();
	void InitIsSprinting();
	void InitAnimInstance();
	// Bind events
	void BindBeginOverlap();
	void BindStaminaDepleted();
	// Bind enhanced input actions
	void BindActions(UEnhancedInputComponent* enhanced);
	void BindActionAttack2(UEnhancedInputComponent* enhanced);
	void BindActionAttack1(UEnhancedInputComponent* enhanced);
	void BindActionRoll(UEnhancedInputComponent* enhanced);
	void BindActionSprint(UEnhancedInputComponent* enhanced);
	void BindActionSprintDeactivate(UEnhancedInputComponent* enhanced);
	void BindActionSprintActivate(UEnhancedInputComponent* enhanced);
	void BindActionMove(UEnhancedInputComponent* enhanced);
	UFUNCTION()
	void OnStaminaDepleted();

	bool IsUsingStamina(float StaminaCost);
	FVector InputToMoveDirection(const FInputActionValue& InValue);
	bool IsAnimMontagePlaying();
	void PlayRoll();
	void RotateActorByLastInput();

	void SpendSprintStamina(float DeltaTime);

	void SetSprintMode();
	void SetWalkMode();

	void PlayAttack1();
	void SetWeaponToAttack();
	void BindMontageEnded();
	void PlayComboAttack1();
	void PlayAttack2();
	void PlayComboAttack2();


	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> ActionAnimInstance = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_AttackTrace> AttackTraceNotify = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_SlashEffect> SlashEffectNotify = nullptr;

	bool bComboReady = false;
};
