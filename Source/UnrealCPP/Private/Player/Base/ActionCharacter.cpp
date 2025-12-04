#include "Player/Base/ActionCharacter.h"

#include "UnrealCPP/UnrealCPP.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Status/Base/ResourceComponent.h"
#include "Status/Base/StatusComponent.h"
#include "Weapon/Manager/WeaponManagerComponent.h"
#include "Weapon/Base/Weapon.h"
#include "Core/Interface/Pickable.h"
#include "Player/Data/PlayerInputData.h"
#include "Player/Data/PlayerMontageData.h"
#include "Combat/AnimNotify/AnimNotifyState_SlashEffect.h"
#include "Item/Pickupable/PickupWeapon.h"
#include "Item/SpawnSystem/PickupFactorySubsystem.h"
#include "Inventory/Base/InventoryComponent.h"

AActionCharacter::AActionCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	InitCameraSystem();
	InitComponents();
	InitDropLocation();
	InitMovementRotation();
}


void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitIsSprinting();
	InitAnimInstance();
	InitResourceByStatus();
	
	BindStaminaDepleted();
	BindBeginOverlap();

	WeaponManager->EquipNewWeapon(EItemCode::EIC_Basic, 10);
}

void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpendSprintStamina(DeltaTime);
}

void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	BindActions(enhanced);
}

void AActionCharacter::InitCameraSystem()
{
	InitSpringArm();
	InitCamera();
}

void AActionCharacter::InitSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SocketOffset = FVector(0, 0, 250);
	SpringArm->bUsePawnControlRotation = true; // SpringArm이 Pawn의 컨트롤 회전을 따르도록 설정
}

void AActionCharacter::InitCamera()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
}

void AActionCharacter::InitComponents()
{
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AActionCharacter::InitDropLocation()
{
	DropLocation = CreateDefaultSubobject<USceneComponent>(TEXT("DropLocation"));
	DropLocation->SetupAttachment(RootComponent);
	DropLocation->SetRelativeLocation(FVector(80.0f, 30.0f, 50.0f));
}

void AActionCharacter::InitMovementRotation()
{
	bUseControllerRotationYaw = false; // Character가 Controller의 Yaw 회전을 따르지 않도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character가 이동 방향으로 회전하도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0); // 회전 속도 설정
}

void AActionCharacter::InitAnimInstance()
{
	if (!GetMesh()) return;
	ActionAnimInstance = GetMesh()->GetAnimInstance();
}

void AActionCharacter::InitResourceByStatus()
{
	if (!ResourceComponent || !StatusComponent) return;
	ResourceComponent->SetAllResourceByStatus(StatusComponent);
}

void AActionCharacter::BindStaminaDepleted()
{
	if (!ResourceComponent) return;
	ResourceComponent->OnStaminaDepleted.AddDynamic(this, &AActionCharacter::OnStaminaDepleted);
}

void AActionCharacter::BindBeginOverlap()
{
	OnActorBeginOverlap.AddDynamic(this, &AActionCharacter::OnBeginOverlap);
}

void AActionCharacter::BindActions(UEnhancedInputComponent* enhanced)
{
	if (!enhanced || !InputData) return;
	BindActionMove(enhanced);
	BindActionSprint(enhanced);
	BindActionRoll(enhanced);
	BindActionAttack(enhanced);
}

void AActionCharacter::BindActionAttack(UEnhancedInputComponent* enhanced)
{
	if (!InputData->Attack) return;
	enhanced->BindAction(InputData->Attack, ETriggerEvent::Triggered, this, &AActionCharacter::OnAttackInput);
}

void AActionCharacter::BindActionRoll(UEnhancedInputComponent* enhanced)
{
	if (!InputData->Roll) return;
	enhanced->BindAction(InputData->Roll, ETriggerEvent::Triggered, this, &AActionCharacter::OnRollInput);
}

void AActionCharacter::BindActionSprint(UEnhancedInputComponent* enhanced)
{
	if (!InputData->Sprint) return;
	BindActionSprintActivate(enhanced);
	BindActionSprintDeactivate(enhanced);
}

void AActionCharacter::BindActionSprintDeactivate(UEnhancedInputComponent* enhanced)
{
	enhanced->BindActionValueLambda(InputData->Sprint, ETriggerEvent::Completed,
		[this](const FInputActionValue& _) { SetWalkMode(); }
	);
}

void AActionCharacter::BindActionSprintActivate(UEnhancedInputComponent* enhanced)
{
	enhanced->BindActionValueLambda(InputData->Sprint, ETriggerEvent::Started,
		[this](const FInputActionValue& _) { SetSprintMode(); }
	);
}

void AActionCharacter::BindActionMove(UEnhancedInputComponent* enhanced)
{
	if (!InputData->Move) return;
	enhanced->BindAction(InputData->Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveInput);
}

inline void AActionCharacter::SetSectionJumpNotify(UAnimNotifyState_SectionJump* InNotify)
{
	SectionJumpNotify = InNotify;
	bComboReady = SectionJumpNotify.IsValid();
}

void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	if (CanMove())
	{
		AddMovementInput(InputToMoveDirection(InValue));
	}
}

bool AActionCharacter::CanMove()
{
	return !IsAnimMontagePlaying();
}

void AActionCharacter::OnRollInput(const FInputActionValue& Value)
{
	if (CanRoll())
	{
		PlayRoll();
	}
}

bool AActionCharacter::CanRoll()
{
	return !GetController()->IsMoveInputIgnored()
		&& !IsAnimMontagePlaying()
		&& ::IsValid(MontageData->Roll);
}

void AActionCharacter::OnAttackInput(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		if (!ActionAnimInstance->IsAnyMontagePlaying())
		{
			PlayAttack();
		}
		else if (ActionAnimInstance->Montage_IsPlaying(MontageData->Attack) && bComboReady)
		{
			bComboReady = false;
			PlayComboAttack();
		}
	}
}

bool AActionCharacter::CanAttack()
{
	return !GetController()->IsMoveInputIgnored()
		&& ActionAnimInstance.IsValid()
		&& ::IsValid(MontageData->Attack)
		&& WeaponManager
		&& WeaponManager->CanAttack()
		&& ResourceComponent->HasEnoughStamina(AttackStaminaCost);
}

FVector AActionCharacter::InputToMoveDirection(const FInputActionValue& InValue)
{
	FVector2D inputDirection = InValue.Get<FVector2D>();
	FVector moveDirection = FVector(inputDirection.Y, inputDirection.X, 0.0f);
	FQuat controlYawRotation = FQuat(FRotator(0, GetControlRotation().Yaw, 0)); 	// 컨트롤러의 Yaw 회전을 기준으로 이동 방향 계산
	return controlYawRotation.RotateVector(moveDirection);
}

bool AActionCharacter::IsAnimMontagePlaying()
{
	return ActionAnimInstance.IsValid() && ActionAnimInstance->IsAnyMontagePlaying();
}

void AActionCharacter::PlayRoll()
{
	if (!IsUsingStamina(RollStaminaCost)) return;

	RotateActorByLastInput();
	PlayAnimMontage(MontageData->Roll);
}

void AActionCharacter::RotateActorByLastInput()
{
	FVector LastMoveDir = GetLastMovementInputVector();
	if (!LastMoveDir.IsNearlyZero())
	{
		SetActorRotation(LastMoveDir.ToOrientationRotator());
	}
}

bool AActionCharacter::IsUsingStamina(float StaminaCost)
{
	if (!ResourceComponent->HasEnoughStamina(StaminaCost)) return false;
	ResourceComponent->UseStamina(StaminaCost);
	return true;
}

void AActionCharacter::SetSprintMode()
{
	if (ResourceComponent->GetStaminaCurrent() <= 0) return;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bIsSprinting = true;
}

void AActionCharacter::SetWalkMode()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprinting = false;
}

void AActionCharacter::SpendSprintStamina(float DeltaTime)
{
	if (!bIsSprinting || GetVelocity().IsNearlyZero()) return;
	if (IsAnimMontagePlaying()) return;

	ResourceComponent->UseStamina(SprintStaminaCost * DeltaTime);
}

void AActionCharacter::PlayAttack()
{
	if (!IsUsingStamina(AttackStaminaCost)) return;
	PlayAnimMontage(MontageData->Attack);
	BindMontageEnded();
	SetWeaponToAttack();
}

void AActionCharacter::SetWeaponToAttack()
{
	WeaponManager->OnAttack();
}

void AActionCharacter::BindMontageEnded()
{
	FOnMontageEnded onMontageEnded;
	onMontageEnded.BindUObject(this, &AActionCharacter::OnAttackMontageEnded);
	ActionAnimInstance->Montage_SetEndDelegate(onMontageEnded);
}

void AActionCharacter::PlayComboAttack()
{
	if (!IsUsingStamina(AttackStaminaCost)) return;
	
	ActionAnimInstance->Montage_JumpToSection(
		SectionJumpNotify.IsValid() ? SectionJumpNotify->GetNextSectionName() : NAME_None,
		MontageData->Attack
	);
	SetWeaponToAttack();
}

void AActionCharacter::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Implements case
	if(OtherActor->Implements<UPickable>())
	{
		IPickable::Execute_OnPickup(OtherActor, this);
	}
}

void AActionCharacter::OnStaminaDepleted()
{
	SetWalkMode();
}

void AActionCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	WeaponManager->ResetWeapon();
}
