// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "InputActionValue.h"
#include "Player/ResourceComponent.h"

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	//TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	TObjectPtr<UResourceComponent> ResourceComponent = nullptr;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprinting = false;

private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> ActionAnimInstance = nullptr;
};
