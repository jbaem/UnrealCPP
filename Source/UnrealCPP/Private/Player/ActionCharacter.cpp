#include "Player/ActionCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/ResourceComponent.h"
#include "Player/StatusComponent.h"
#include "Player/WeaponManagerComponent.h"
#include "Weapon/Weapon.h"
#include "Item/Pickable.h"
#include "Item/PickupWeapon.h"
#include "Player/PlayerInputData.h"
#include "Player/PlayerData.h"
#include "Player/PlayerMontageData.h"
#include "AnimNotify/AnimNotifyState_SlashEffect.h"
#include "Framework/PickupFactorySubsystem.h"

AActionCharacter::AActionCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	InitCameraSystem();
	InitComponents();
	InitDropLocation();
	InitMovementRotation();
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

void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitIsSprinting();
	InitAnimInstance();
	InitResourceByStatus();
	
	BindStaminaDepleted();
	BindBeginOverlap();

	EquipNewWeapon(EItemCode::EIC_Basic, 10);
}

void AActionCharacter::InitIsSprinting()
{
	bIsSprinting = false;
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

void AActionCharacter::BindActions(UEnhancedInputComponent* enhanced)
{
	if (!enhanced || !InputData) return;
	
	BindActionMove(enhanced);
	BindActionSprint(enhanced);
	BindActionRoll(enhanced);
	BindActionAttack1(enhanced);
	BindActionAttack2(enhanced);
}

void AActionCharacter::BindActionAttack2(UEnhancedInputComponent* enhanced)
{
	if (!InputData->Attack2) return;
	enhanced->BindAction(InputData->Attack2, ETriggerEvent::Triggered, this, &AActionCharacter::OnAttack2Input);
}

void AActionCharacter::BindActionAttack1(UEnhancedInputComponent* enhanced)
{
	if (!InputData->Attack1) return;
	enhanced->BindAction(InputData->Attack1, ETriggerEvent::Triggered, this, &AActionCharacter::OnAttack1Input);
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

void AActionCharacter::AddItem_Implementation(EItemCode Code, int32 Count)
{
	//EquipWeapon(Code, Count);
}

void AActionCharacter::AddWeapon_Implementation(EItemCode Code, int32 AttackCount)
{
	EquipWeapon(Code, AttackCount);
}

inline void AActionCharacter::SetSectionJumpNotify(UAnimNotifyState_SectionJump* InNotify)
{
	SectionJumpNotify = InNotify;
	bComboReady = SectionJumpNotify.IsValid();
}

inline void AActionCharacter::SetAttackTraceNotify(UAnimNotifyState_AttackTrace* InNotify)
{
	AttackTraceNotify = InNotify;
	PlayerWeapon->AttackEnable(AttackTraceNotify.IsValid());
}

inline void AActionCharacter::SetSlashEffectNotify(UAnimNotifyState_SlashEffect* InNotify)
{
	SlashEffectNotify = InNotify;
	PlayerWeapon->ActivateSlashEffect(SlashEffectNotify.IsValid());
}

void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	if (IsAnimMontagePlaying()) return;
	AddMovementInput(InputToMoveDirection(InValue));
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

void AActionCharacter::OnRollInput(const FInputActionValue& Value)
{
	if (IsAnimMontagePlaying()) return;
	if (!::IsValid(MontageData->Roll)) return;
	
	PlayRoll();
}

void AActionCharacter::PlayRoll()
{
	if (!IsUsingStamina(PlayerData->RollStaminaCost)) return;

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

void AActionCharacter::OnAttack1Input(const FInputActionValue& Value)
{
	if (!ActionAnimInstance.IsValid() || !::IsValid(MontageData->Attack1)) return;
	if (!PlayerWeapon || !PlayerWeapon->CanAttack()) return;

	if (!ResourceComponent->HasEnoughStamina(PlayerData->AttackStaminaCost)) return;
	
	if (!ActionAnimInstance->IsAnyMontagePlaying())
	{
		PlayAttack1();
	}
	else if(ActionAnimInstance->Montage_IsPlaying(MontageData->Attack1) && bComboReady)
	{
		bComboReady = false;
		PlayComboAttack1();
	}
}

void AActionCharacter::OnAttack2Input(const FInputActionValue& Value)
{
	if (!ActionAnimInstance.IsValid() || !::IsValid(MontageData->Attack2)) return;
	if (!ResourceComponent->HasEnoughStamina(PlayerData->Attack2StaminaCost)) return;

	if (!ActionAnimInstance->IsAnyMontagePlaying())
	{
		PlayAttack2();
	}
	else if (ActionAnimInstance->Montage_IsPlaying(MontageData->Attack2) && bComboReady)
	{
		bComboReady = false;
		PlayComboAttack2();
	}
}

void AActionCharacter::SetSprintMode()
{
	if (ResourceComponent->GetStaminaCurrent() <= 0) return;
	GetCharacterMovement()->MaxWalkSpeed = PlayerData->SprintSpeed;
	bIsSprinting = true;
}

void AActionCharacter::SetWalkMode()
{
	GetCharacterMovement()->MaxWalkSpeed = PlayerData->WalkSpeed;
	bIsSprinting = false;
}

void AActionCharacter::SpendSprintStamina(float DeltaTime)
{
	if (!bIsSprinting || GetVelocity().IsNearlyZero()) return;
	if (IsAnimMontagePlaying()) return;

	ResourceComponent->UseStamina(PlayerData->SprintStaminaCost * DeltaTime);
}

void AActionCharacter::PlayAttack1()
{
	if (!IsUsingStamina(PlayerData->AttackStaminaCost)) return;
	PlayAnimMontage(MontageData->Attack1);
	BindMontageEnded();
	SetWeaponToAttack();
}

void AActionCharacter::SetWeaponToAttack()
{
	if (!PlayerWeapon) return;
	PlayerWeapon->OnAttack();
}

void AActionCharacter::BindMontageEnded()
{
	FOnMontageEnded onMontageEnded;
	onMontageEnded.BindUObject(this, &AActionCharacter::OnAttackMontageEnded);
	ActionAnimInstance->Montage_SetEndDelegate(onMontageEnded);
}

void AActionCharacter::PlayComboAttack1()
{
	if (!IsUsingStamina(PlayerData->AttackStaminaCost)) return;
	
	ActionAnimInstance->Montage_JumpToSection(
		SectionJumpNotify.IsValid() ? SectionJumpNotify->GetNextSectionName() : NAME_None,
		MontageData->Attack1
	);
	SetWeaponToAttack();
}

void AActionCharacter::PlayAttack2()
{
	if (!IsUsingStamina(PlayerData->Attack2StaminaCost)) return;
	PlayAnimMontage(MontageData->Attack2);
	SetWeaponToAttack();
}

void AActionCharacter::PlayComboAttack2()
{
	if (!IsUsingStamina(PlayerData->Attack2StaminaCost)) return;

	ActionAnimInstance->Montage_JumpToSection(
		SectionJumpNotify.IsValid() ? SectionJumpNotify->GetNextSectionName() : NAME_None,
		MontageData->Attack2
	);
	SetWeaponToAttack();
}

void AActionCharacter::OnAreaAttack()
{
	if (::IsValid(PlayerWeapon))
	{
		PlayerWeapon->DamageToArea();
	}
}

void AActionCharacter::EquipWeapon(EItemCode WeaponCode, int32 Count)
{
	if (!::IsValid(PlayerWeapon)) return;
	
	PlayerWeapon->WeaponActivate(false);

	if (ShouldDropCurrentWeapon(WeaponCode))
	{
		DropCurrentWeapon();
	}
	EquipNewWeapon(WeaponCode, Count);
}

void AActionCharacter::EquipNewWeapon(EItemCode WeaponCode, int32 Count)
{
	PlayerWeapon = WeaponManager->GetEquippedWeaponByItemCode(WeaponCode);
	PlayerWeapon->WeaponActivate(true);
	PlayerWeapon->SetUsedCountRemain(Count);
}

bool AActionCharacter::ShouldDropCurrentWeapon(EItemCode WeaponCode)
{
	return WeaponCode != EItemCode::EIC_Basic && WeaponCode != PlayerWeapon->GetWeaponID();
}

void AActionCharacter::DropWeapon(EItemCode WeaponCode)
{
	UPickupFactorySubsystem* subsystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();
	if (subsystem)
	{
		subsystem->SpawnUsedWeaponByItemCode(
			WeaponCode,
			DropLocation->GetComponentLocation(),
			GetActorRotation()
		);
	}
}

void AActionCharacter::DropCurrentWeapon()
{
	UPickupFactorySubsystem* subsystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();
	if (subsystem)
	{
		int32 usedCount = PlayerWeapon->GetUsedCountRemain();

		APickup* pickup = subsystem->SpawnCurrentWeaponByItemCode(
			PlayerWeapon->GetWeaponID(),
			DropLocation->GetComponentLocation(),
			GetActorRotation(),
			(GetActorForwardVector() + GetActorUpVector()) * 300.0f
		);
		
		if (!pickup) return;

		Cast<APickupWeapon>(pickup)->SetAttackCountRemain(usedCount);
	}
}

void AActionCharacter::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	/* // Cast case
	IPickable* pickableItem = Cast<IPickable>(OtherActor);
	if(pickableItem)
	{
		IPickable::Execute_OnPickup(OtherActor);	// Execute blueprint if implemented
		//pickableItem->OnPickup_Implementation();	// Execute only C++ implementation directly
	}
	*/

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
	if (::IsValid(PlayerWeapon) && !PlayerWeapon->CanAttack())
	{
		DropWeapon(PlayerWeapon->GetWeaponID());
		EquipWeapon(EItemCode::EIC_Basic, 10);
	}
}
