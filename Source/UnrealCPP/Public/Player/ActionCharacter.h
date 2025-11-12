// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "InputActionValue.h"

#include "ActionCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnMoveInput(const FInputActionValue& Value);
	void OnRollInput(const FInputActionValue& Value);

	void SetSprintMode();
	void SetWalkMode();

	void UseStamina(float StaminaCost);
	void StartStaminaRegenTimer();
	void RegenStaminaPerTick();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	//TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Roll = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Walk")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Sprint")
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Sprint")
	float SprintStaminaCost = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Roll")
	float RollStaminaCost = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Animation|Montage")
	TObjectPtr<UAnimMontage> RollMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Resource|Stamina")
	float StaminaMax = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Resource|Stamina")
	float StaminaCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Resource|Stamina")
	float StaminaRegenAmountPerTick = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource|Stamina")
	float StaminaRegenCoolTime = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprinting = false;

private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> ActionAnimInstance = nullptr;

	FTimerHandle StaminaRegenTimerHandle;
};
