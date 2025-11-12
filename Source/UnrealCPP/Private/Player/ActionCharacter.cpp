// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SocketOffset = FVector(0, 0, 250);
	SpringArm->bUsePawnControlRotation = true; // SpringArm이 Pawn의 컨트롤 회전을 따르도록 설정
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	
	bUseControllerRotationYaw = false; // Character가 Controller의 Yaw 회전을 따르지 않도록 설정
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character가 이동 방향으로 회전하도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0); // 회전 속도 설정

	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Get Animation Blueprint Instance
	ActionAnimInstance = GetMesh()->GetAnimInstance(); 

	bIsSprinting = false;
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting && !GetLastMovementInputVector().IsNearlyZero())
	{
		ResourceComponent->UseStamina(SprintStaminaCost * DeltaTime);
	}


	// 로그 시간 찍으면서 스태미나 확인
	//UE_LOG(LogTemp, Log, TEXT("Time : %.1f  || Stamina : %.1f / %.1f"), GetWorld()->GetTimeSeconds(), StaminaCurrent, StaminaMax);
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhanced)
	{
		//여기에 Input Action Bindings 추가
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveInput);
		
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started, 
			[this](const FInputActionValue& _) {
				SetSprintMode();
			}
		);
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed,
			[this](const FInputActionValue& _) {
				SetWalkMode();
			}
		);

		enhanced->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AActionCharacter::OnRollInput);
	}
}


void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	FVector2D inputDirection = InValue.Get<FVector2D>();
	
	// 컨트롤러의 Yaw 회전을 기준으로 이동 방향 계산
	FVector moveDirection = FVector(inputDirection.Y, inputDirection.X, 0.0f);
	FQuat controlYawRotation = FQuat(FRotator(0, GetControlRotation().Yaw, 0));

	moveDirection = controlYawRotation.RotateVector(moveDirection);
	AddMovementInput(moveDirection);

	// Forward/Right 벡터를 사용하여 이동
	/*
	//const FRotator controlRotation = GetControlRotation();
	//const FRotator yawRotation(0.0f, controlRotation.Yaw, 0.0f);
	//const FVector forward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	//AddMovementInput(forward, inputDirection.Y);
	//const FVector right = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	//AddMovementInput(right, inputDirection.X);	
	*/

	//UE_LOG(LogTemp, Log, TEXT("Dir : (%.1f, %.1f)"), inputDirection.X, inputDirection.Y);
	//UE_LOG(LogTemp, Log, TEXT("Dir : (%s)"), *inputDirection.ToString());
}

void AActionCharacter::OnRollInput(const FInputActionValue& Value)
{
	if (!ActionAnimInstance.IsValid() || !::IsValid(RollMontage)) return;
	
	if (ActionAnimInstance->IsAnyMontagePlaying()) return;
	
	if (!ResourceComponent->HasEnoughStamina(RollStaminaCost)) return;

	ResourceComponent->UseStamina(RollStaminaCost);

	FVector LastMoveDir = GetLastMovementInputVector();
	//UE_LOG(LogTemp, Log, TEXT("Roll Input"));
	//UE_LOG(LogTemp, Log, TEXT("Last Movement Input Vector : %s"), *LastMoveDir.ToString());
	
	if (!LastMoveDir.IsNearlyZero())
	{
		SetActorRotation(LastMoveDir.ToOrientationRotator());
	}
	PlayAnimMontage(RollMontage);
	
}

void AActionCharacter::SetSprintMode()
{
	//UE_LOG(LogTemp, Log, TEXT("Sprint Mode"));
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bIsSprinting = true;
}

void AActionCharacter::SetWalkMode()
{
	//UE_LOG(LogTemp, Log, TEXT("Walk Mode"));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprinting = false;
}
