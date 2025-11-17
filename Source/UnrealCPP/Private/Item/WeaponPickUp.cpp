// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponPickUp.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Player/InventoryOwner.h"
#include "Components/TimelineComponent.h"

// Sets default values
AWeaponPickUp::AWeaponPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseRoot = CreateDefaultSubobject<USphereComponent>(TEXT("BaseRoot"));
	SetRootComponent(BaseRoot);
	BaseRoot->SetSimulatePhysics(true);
	BaseRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BaseRoot->SetSphereRadius(1.0f);

	// Constraints
	BaseRoot->BodyInstance.bLockXRotation = true;
	BaseRoot->BodyInstance.bLockYRotation = true;
	BaseRoot->BodyInstance.bLockZRotation = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(BaseRoot);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	//TODO: 적용 안 됨
	SkeletalMesh->AddRelativeRotation(FRotator(0, 0, -10.0f));

	PickupOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap"));
	PickupOverlap->SetupAttachment(BaseRoot);
	PickupOverlap->SetSphereRadius(100.0f);
	SkeletalMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(BaseRoot);

	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
}

// Called when the game starts or when spawned
void AWeaponPickUp::BeginPlay()
{
	Super::BeginPlay();

	if(PickupOverlap)
	{
		PickupOverlap->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickUp::OnPickupBeginOverlap);
	}

	if (PickupTimeline)
	{
		if (ScaleCurve)
		{
			FOnTimelineFloat ScaleUpdateDelegate;
			ScaleUpdateDelegate.BindUFunction(this, FName("HandleScaleProgress"));
			PickupTimeline->AddInterpFloat(ScaleCurve, ScaleUpdateDelegate);
			
			FOnTimelineEvent ScaleFinishedDelegate;
			ScaleFinishedDelegate.BindUFunction(this, FName("OnScaleFinished"));
			PickupTimeline->SetTimelineFinishedFunc(ScaleFinishedDelegate);
		}

		PickupTimeline->SetPlayRate(1.0f / PickupDuration);
	}

	bIsPickedUp = false;
}

// Called every frame
void AWeaponPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	SkeletalMesh->AddWorldRotation(FRotator(0, RotateSpeed * DeltaTime, 0));

}

void AWeaponPickUp::OnPickup_Implementation(AActor* Target)
{
	if(bIsPickedUp)
	{
		return;
	}
	bIsPickedUp = true;

	PickupTarget = Target;
	//UE_LOG(LogTemp, Log, TEXT("Weapon Picked Up"));
	PickupTimeline->PlayFromStart();
}

void AWeaponPickUp::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//UE_LOG(LogTemp, Log, TEXT("OnPickupBeginOverlap"));
}

void AWeaponPickUp::HandleScaleProgress(float Value)
{
	FVector newScale = FVector(Value, Value, Value);
	SkeletalMesh->SetRelativeScale3D(newScale);
}

void AWeaponPickUp::OnScaleFinished()
{
	if(PickupTarget.IsValid() && PickupTarget->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(PickupTarget.Get(), ItemCode);
		//Destroy();
	}
}
