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
class UNREALCPP_API AActionCharacter : public ACharacter
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
	// notify setters
	inline void SetSectionJumpNotify(class UAnimNotifyState_SectionJump* InNotify);

protected:
	// Input handlers
	void OnMoveInput(const FInputActionValue& Value);
	bool CanMove();
	void OnRollInput(const FInputActionValue& Value);
	bool CanRoll();
	void OnAttackInput(const FInputActionValue& Value);
	bool CanAttack();

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	USceneComponent* GetDropLocation() const { return DropLocation; }
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
	// Managers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TObjectPtr<class UWeaponManagerComponent> WeaponManager = nullptr;
	// States
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprinting = false;

private:
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

	void PlayAttack();
	void SetWeaponToAttack();
	void BindMontageEnded();
	void PlayComboAttack();

	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> ActionAnimInstance = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify = nullptr;

	bool bComboReady = false;
	
	// Init functions
	// Constructor settings
	void InitCameraSystem();
	void InitComponents();
	void InitDropLocation();
	void InitMovementRotation();
	void InitSpringArm();
	void InitCamera();
	// BeginPlay settings
	void InitResourceByStatus();
	inline void InitIsSprinting() { bIsSprinting = false; };
	void InitAnimInstance();
	// Bind events
	void BindBeginOverlap();
	void BindStaminaDepleted();
	// Bind enhanced input actions
	void BindActions(UEnhancedInputComponent* enhanced);
	void BindActionAttack(UEnhancedInputComponent* enhanced);
	void BindActionRoll(UEnhancedInputComponent* enhanced);
	void BindActionSprint(UEnhancedInputComponent* enhanced);
	void BindActionSprintDeactivate(UEnhancedInputComponent* enhanced);
	void BindActionSprintActivate(UEnhancedInputComponent* enhanced);
	void BindActionMove(UEnhancedInputComponent* enhanced);
};
