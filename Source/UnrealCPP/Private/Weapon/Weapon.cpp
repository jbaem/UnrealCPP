#include "Weapon/Weapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
// Kismet = Unreal의 시각적 스크립팅 시스템인 블루프린트와 상호작용하기 위한 다양한 유틸리티 함수들을 제공하는 모듈
#include "Kismet/GameplayStatics.h"
#include <Player/StatusComponent.h>
#include <TimerManager.h>
#include "Player/ActionCharacter.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetupAttachment(root);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	//WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	
	WeaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponEffect"));
	WeaponEffect->SetupAttachment(WeaponMesh);
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Class Default Object(CDO) : Unreal Engine에서 각 클래스에 대해 하나씩 존재하는 특별한 객체로, 해당 클래스의 기본 속성값들을 보유
	// OverlapOnlyPawn 설정 이후에 NoCollision 설정
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);
}

void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	float finalDamage = Damage;
	AController* instigator = nullptr;
	
	if (WeaponOwner.IsValid())
	{
		if(WeaponOwner == OtherActor)
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

	UGameplayStatics::ApplyDamage(OtherActor, finalDamage, instigator, this, DamageType);
}

void AWeapon::DestroyWeapon()
{
	// TODO: 캐릭터에게 알려야 함.
}

void AWeapon::WeaponActivate(bool bActivate)
{
	/*
	* < Activate / Deactivate >
	* Visibility - o
	* Collision - x
	* Actor's Tick - x
	* Actor's Components Tick - x
	* Physics Simulation - x
	* Timer - x
	* Audio - x
	* Particle System - x
	* Animation - x
	* Movement Component - x
	* AI Logic - x
	*/

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

