#include "Weapon/Base/Weapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
// Kismet = Unreal의 시각적 스크립팅 시스템인 블루프린트와 상호작용하기 위한 다양한 유틸리티 함수들을 제공하는 모듈
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Player/Base/ActionCharacter.h"
#include "Status/Base/StatusComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	InitRoot();
	InitMeshComponent();
	InitCapsuleCollision();
	InitVFX();
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Class Default Object(CDO) : Unreal Engine에서 각 클래스에 대해 하나씩 존재하는 특별한 객체로, 해당 클래스의 기본 속성값들을 보유
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);
	WeaponEffect->Deactivate();
}

void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	DamageToTarget(OtherActor);
}

void AWeapon::DamageToTarget(AActor* Target)
{
	float finalDamage = Damage;
	AController* instigator = nullptr;
	
	if (WeaponOwner.IsValid())
	{
		if(WeaponOwner == Target)
		{
			return;
		}
		
		UStatusComponent* statusComponent = 
			WeaponOwner->GetComponentByClass(UStatusComponent::StaticClass()) 
			? Cast<UStatusComponent>(WeaponOwner->GetComponentByClass(UStatusComponent::StaticClass())) 
			: nullptr;

		if (statusComponent)
		{
			finalDamage += statusComponent->GetAttackPower();
		}
		instigator = WeaponOwner->GetController();
	}
	UGameplayStatics::ApplyDamage(Target, finalDamage, instigator, this, DamageType);
}

void AWeapon::DamageToArea()
{
	float finalDamage = Damage;
	AController* instigator = nullptr;
	if (WeaponOwner.IsValid())
	{
		AActionCharacter* weaponOwner = Cast<AActionCharacter>(WeaponOwner);
		if (weaponOwner->GetStatusComponent() != nullptr)
		{
			finalDamage += weaponOwner->GetStatusComponent()->GetAttackPower();
		}
		instigator = WeaponOwner->GetController();
	}
	finalDamage *= 2.0f; // Area damage is stronger

	FVector center = FMath::Lerp(
		WeaponMesh->GetSocketLocation(TEXT("Blade_Base")),
		WeaponMesh->GetSocketLocation(TEXT("Blade_Tip")),
		0.5f
	);

	DrawDebugSphere(
		GetWorld(),
		center,
		AreaInnerRadius,
		12,
		FColor::Red,
		false,
		DebugDurtaion,
		0,
		1.0f
	);

	DrawDebugSphere(
		GetWorld(),
		center,
		AreaOuterRadius,
		12,
		FColor::Yellow,
		false,
		DebugDurtaion,
		0,
		1.0f
	);

	if(AreaAttackEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			AreaAttackEffect,
			center,
			WeaponOwner->GetActorRotation()
		);
	}

	TArray<AActor*> ignoreActors = { WeaponOwner.Get(), this };
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		finalDamage,
		Damage,
		center,
		AreaInnerRadius,
		AreaOuterRadius,
		Falloff,	//거리에 따른 데미지 감소 정도
		DamageType,
		ignoreActors,
		this,
		instigator,
		ECollisionChannel::ECC_Pawn
	);

	if (AreaAttackEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			AreaAttackEffect,
			center,
			WeaponOwner->GetActorRotation()
		);
	}

	// 범위 내 고정 데미지
	//UGameplayStatics::ApplyRadialDamage(
	//	this,
	//	finalDamage,
	//	center,
	//	AreaOuterRadius,
	//	DamageType,
	//	ignoreActors,
	//	this,
	//	instigator,
	//	true,
	//	ECC_Visibility
	//);
}

void AWeapon::WeaponActivate(bool bActivate)
{
	if(bActivate)
	{
		AttachToComponent(
			WeaponOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("hand_rSocket")
		);
		SetActorHiddenInGame(false);

		OnWeaponActivate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Deactivate"));
		SetActorHiddenInGame(true);
		AttachToComponent(
			WeaponOwner->GetMesh(),
			FAttachmentTransformRules::KeepWorldTransform,
			FName("root") // 임시로 root 소켓에 부착
		);
		SetActorRelativeLocation(FVector(0, 0, -10000));

		OnWeaponDeactivate();
	}
}

void AWeapon::AttackEnable(bool bEnable)
{
	WeaponCollision->SetCollisionEnabled(
		bEnable ? 
		ECollisionEnabled::QueryOnly : 
		ECollisionEnabled::NoCollision);
}

void AWeapon::ActivateSlashEffect(bool bAtivate)
{
	if (WeaponEffect)
	{
		if (bAtivate)
		{
			WeaponEffect->Activate(true);
		}
		else
		{
			WeaponEffect->Deactivate();
		}
	}
}

void AWeapon::StartHoldingTimer()
{
	UWorld* world = GetWorld();
	if (!world) return;

	FTimerManager& timerManager = world->GetTimerManager();

	timerManager.ClearTimer(HoldingTimerHandle);
	timerManager.SetTimer(
		HoldingTimerHandle, 
		this, 
		&AWeapon::DestroyWeapon, 
		HoldingDuration, 
		false
	);
}

void AWeapon::OnWeaponPickuped()
{
}

void AWeapon::InitRoot()
{
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);
}

void AWeapon::InitVFX()
{
	WeaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponEffect"));
	WeaponEffect->SetupAttachment(WeaponMesh);
}

void AWeapon::InitCapsuleCollision()
{
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void AWeapon::InitMeshComponent()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	//WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
