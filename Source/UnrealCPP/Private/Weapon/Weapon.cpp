// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
// Kismet = Unreal의 시각적 스크립팅 시스템인 블루프린트와 상호작용하기 위한 다양한 유틸리티 함수들을 제공하는 모듈
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
}

// Called when the game starts or when spawned
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
		
		//float finalDamage += WeaponOwner->GetAttackPower();
		instigator = WeaponOwner->GetController();
	}

	UGameplayStatics::ApplyDamage(OtherActor, finalDamage, instigator, this, DamageType);

	UE_LOG(LogTemp, Log, TEXT("Weapon Overlap with %s"), *OtherActor->GetName());
}

