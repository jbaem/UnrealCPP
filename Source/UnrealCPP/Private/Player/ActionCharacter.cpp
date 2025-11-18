// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/ResourceComponent.h"
#include "Player/StatusComponent.h"
#include "Weapon/Weapon.h"
#include "Item/Pickable.h"


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
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Get Animation Blueprint Instance
	if (GetMesh())
	{
		ActionAnimInstance = GetMesh()->GetAnimInstance(); 
	}

	if (ResourceComponent)
	{
		ResourceComponent->OnStaminaDepleted.AddDynamic(this, &AActionCharacter::OnStaminaDepleted);
	}

	bIsSprinting = false;


	if (ResourceComponent && StatusComponent)
	{
		ResourceComponent->SetAllResourceByStatus(StatusComponent);
		//UE_LOG(LogTemp, Log, TEXT("Strength : %d, Agility : %d, Vitality : %d"),
		//	StatusComponent->GetStrength(), StatusComponent->GetAgility(), StatusComponent->GetVitality());
		//UE_LOG(LogTemp, Log, TEXT("HealthMax : %.1f, StaminaMax : %.1f"),
		//	ResourceComponent->GetHealthMax(), ResourceComponent->GetStaminaMax());
	}

	// Register overlap event
	OnActorBeginOverlap.AddDynamic(this, &AActionCharacter::OnBeginOverlap);

	EquipWeapon();
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpendSprintStamina(DeltaTime);
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
	
		enhanced->BindAction(IA_Attack1, ETriggerEvent::Triggered, this, &AActionCharacter::OnAttack1Input);
		enhanced->BindAction(IA_Attack2, ETriggerEvent::Triggered, this, &AActionCharacter::OnAttack2Input);
	}
}

void AActionCharacter::AddItem_Implementation(EItemCode Code)
{
	const UEnum* EnumPtr = StaticEnum<EItemCode>();
	UE_LOG(LogTemp, Log, TEXT("ActionCharacter AddItem : %s"), *UEnum::GetValueAsString(Code));

}

inline void AActionCharacter::SetAttackTraceNotify(UAnimNotifyState_AttackTrace* InNotify)
{
	AttackTraceNotify = InNotify;
	PlayerWeapon->AttackEnable(AttackTraceNotify.IsValid());
}

void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	if(ActionAnimInstance.IsValid() && ActionAnimInstance->IsAnyMontagePlaying())
	{
		return;
	}

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
	
	if (!ResourceComponent->HasEnoughStamina(RollStaminaCost)) return;
	
	if (ActionAnimInstance->IsAnyMontagePlaying()) return;

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

void AActionCharacter::OnAttack1Input(const FInputActionValue& Value)
{
	if (!ActionAnimInstance.IsValid() || !::IsValid(AttackMontage)) return;
	if (!ResourceComponent->HasEnoughStamina(AttackStaminaCost)) return;
	if (!PlayerWeapon->CanAttack()) return;

	if (!ActionAnimInstance->IsAnyMontagePlaying())
	{
		PlayAttack1();
	}
	else if(ActionAnimInstance->Montage_IsPlaying(AttackMontage) && bComboReady)
	{
		bComboReady = false;
		PlayComboAttack1();
	}
}

void AActionCharacter::OnAttack2Input(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Attack2 Input"));
	if (!ActionAnimInstance.IsValid() || !::IsValid(Attack2Montage)) return;

	if (!ResourceComponent->HasEnoughStamina(Attack2StaminaCost)) return;

	//UE_LOG(LogTemp, Log, TEXT("Has Enough Stamina"));

	if (!ActionAnimInstance->IsAnyMontagePlaying())
	{
		PlayAttack2();
	}
	else if (ActionAnimInstance->Montage_IsPlaying(Attack2Montage) && bComboReady)
	{
		bComboReady = false;
		PlayComboAttack2();
	}
}

void AActionCharacter::SetSprintMode()
{
	if (ResourceComponent->GetStaminaCurrent() <= 0) return;
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

void AActionCharacter::SpendSprintStamina(float DeltaTime)
{
	if (bIsSprinting && !GetVelocity().IsNearlyZero())
	{
		if(ActionAnimInstance.IsValid() && ActionAnimInstance->IsAnyMontagePlaying())
		{
			return;
		}
		ResourceComponent->UseStamina(SprintStaminaCost * DeltaTime);

		//UE_LOG(LogTemp, Warning, TEXT("Stamina : %.1f / %.1f"),
		//	ResourceComponent ? ResourceComponent->GetStaminaCurrent() : 0.0f,
		//	ResourceComponent ? ResourceComponent->GetStaminaMax() : 0.0f
		//);
	}

}

void AActionCharacter::PlayAttack1()
{
	ResourceComponent->UseStamina(AttackStaminaCost);
	PlayAnimMontage(AttackMontage);

	FOnMontageEnded onMontageEnded;
	onMontageEnded.BindUObject(this, &AActionCharacter::OnAttackMontageEnded);
	ActionAnimInstance->Montage_SetEndDelegate(onMontageEnded);

	if (PlayerWeapon)
	{
		PlayerWeapon->OnAttack();
	}
}

void AActionCharacter::PlayComboAttack1()
{
	ResourceComponent->UseStamina(AttackStaminaCost);

	ActionAnimInstance->Montage_JumpToSection(
		SectionJumpNotify.IsValid() ? SectionJumpNotify->GetNextSectionName() : NAME_None,
		AttackMontage
	);

	//UAnimMontage* current = ActionAnimInstance->GetCurrentActiveMontage();
	//ActionAnimInstance->Montage_SetNextSection(
	//	ActionAnimInstance->Montage_GetCurrentSection(current),
	//	SectionJumpNotify.IsValid() ? SectionJumpNotify->GetNextSectionName() : NAME_None,
	//	AttackMontage
	//);

	if (PlayerWeapon)
	{
		PlayerWeapon->OnAttack();
	}
}

void AActionCharacter::PlayAttack2()
{
	ResourceComponent->UseStamina(Attack2StaminaCost);
	PlayAnimMontage(Attack2Montage);

	if (PlayerWeapon)
	{
		PlayerWeapon->OnAttack();
	}
}

void AActionCharacter::PlayComboAttack2()
{
	ResourceComponent->UseStamina(Attack2StaminaCost);

	ActionAnimInstance->Montage_JumpToSection(
		SectionJumpNotify.IsValid() ? SectionJumpNotify->GetNextSectionName() : NAME_None,
		Attack2Montage
	);
	
	if (PlayerWeapon)
	{
		PlayerWeapon->OnAttack();
	}
}

void AActionCharacter::EquipWeapon()
{
	if (PlayerWeapon || !DefaultWeaponClass) return;

	UWorld* world = GetWorld();
	if (!world) return;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	AWeapon* NewWeapon = world->SpawnActor<AWeapon>(DefaultWeaponClass, spawnParams);
	if(NewWeapon)
	{
		NewWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			FName("hand_rSocket"));

		PlayerWeapon = NewWeapon;
		PlayerWeapon->OnWeaponPickuped(this);
	}
}

void AActionCharacter::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Log, TEXT("Character OnBeginOverlap : other is %s"), *OtherActor->GetName());

	// Cast case
	//IPickable* pickableItem = Cast<IPickable>(OtherActor);
	//if(pickableItem)
	//{
	//	IPickable::Execute_OnPickup(OtherActor);	// Execute blueprint if implemented
	//	//pickableItem->OnPickup_Implementation();	// Execute only C++ implementation directly
	//}

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
	UE_LOG(LogTemp, Log, TEXT("Attack Montage Ended"));
	UE_LOG(LogTemp, Log, TEXT("AttackCount : %d"), PlayerWeapon->GetUsedCountRemain());
	if (::IsValid(PlayerWeapon) && !PlayerWeapon->CanAttack())
	{
		TSubclassOf<AActor>* usedClass = UsedWeapon.Find(PlayerWeapon->GetWeaponID());

		AActor* used = GetWorld()->SpawnActor<AActor>(
			*usedClass,
			GetActorLocation() + GetActorForwardVector() * 100.0f,
			FRotator()
		);

		UPrimitiveComponent* primitive = used->FindComponentByClass<UPrimitiveComponent>();
		primitive->AddImpulse((GetActorForwardVector() + GetActorUpVector()) * 500.0f, NAME_None, true);

		// TODO: 기본 무기 장착
	}
}
