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

	void InitMovementRotation();
	void InitDropLocation();
	void InitCameraSystem();
	void InitCamera();
	void InitSpringArm();
	void InitComponents();

protected:
	virtual void BeginPlay() override;

	void BindBeginOverlap();

	void InitResourceByStatus();

	void InitIsSprinting();

	void InitAnimInstance();

	void BindStaminaDepleted();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BindActions(class UEnhancedInputComponent* enhanced);

	void BindActionAttack2(UEnhancedInputComponent* enhanced);

	void BindActionAttack1(UEnhancedInputComponent* enhanced);

	void BindActionRoll(UEnhancedInputComponent* enhanced);

	void BindActionSprint(UEnhancedInputComponent* enhanced);

	void BindActionSprintDeactivate(UEnhancedInputComponent* enhanced);

	void BindActionSprintActivate(UEnhancedInputComponent* enhanced);

	void BindActionMove(UEnhancedInputComponent* enhanced);

	virtual void AddItem_Implementation(EItemCode Code, int32 Count);

	inline void SetSectionJumpNotify(class UAnimNotifyState_SectionJump* InNotify);
	inline void SetAttackTraceNotify(class UAnimNotifyState_AttackTrace* InNotify);
	inline void SetSlashEffectNotify(class UAnimNotifyState_SlashEffect* InNotify);
	
	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void EquipWeapon(EItemCode WeaponCode, int32 Count);

	void EquipNewWeapon(EItemCode WeaponCode, int32 Count);

	bool ShouldDropCurrentWeapon(EItemCode WeaponCode);

	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void DropWeapon(EItemCode WeaponCode);

	UFUNCTION(BlueprintCallable, Category = "Player|Weapon")
	void DropCurrentWeapon();

	void OnAreaAttack();

protected:
	void OnMoveInput(const FInputActionValue& Value);
	FVector InputToMoveDirection(const FInputActionValue& InValue);
	bool IsAnimMontagePlaying();
	void OnRollInput(const FInputActionValue& Value);
	void PlayRoll();
	void RotateActorByLastInput();
	bool IsUsingStamina(float StaminaCost);
	void OnAttack1Input(const FInputActionValue& Value);
	void OnAttack2Input(const FInputActionValue& Value);

	void SetSprintMode();
	void SetWalkMode();

	void SpendSprintStamina(float DeltaTime);

	void PlayAttack1();
	void SetWeaponToAttack();
	void BindMontageEnded();
	void PlayComboAttack1();
	void PlayAttack2();
	void PlayComboAttack2();

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
	// Manager
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TObjectPtr<class UWeaponManagerComponent> WeaponManager = nullptr;
	// Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UPlayerInputData> InputData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UPlayerData> PlayerData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<class UPlayerMontageData> MontageData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	TObjectPtr<AWeapon> PlayerWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprinting = false;

private:
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
