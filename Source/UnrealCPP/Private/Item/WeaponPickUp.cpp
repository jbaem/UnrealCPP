#include "Item/WeaponPickUp.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Player/InventoryOwner.h"

AWeaponPickUp::AWeaponPickUp()
{
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

	if (PickupTimeline)
	{
		if (DistanceCurve)
		{
			FOnTimelineFloat UpdateDelegate;
			UpdateDelegate.BindUFunction(this, FName("OnTimelineUpdate"));
			PickupTimeline->AddInterpFloat(DistanceCurve, UpdateDelegate);
			
			FOnTimelineEvent FinishedDelegate;
			FinishedDelegate.BindUFunction(this, FName("OnTimelineFinished"));
			PickupTimeline->SetTimelineFinishedFunc(FinishedDelegate);
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
	PickupStartLocation = GetActorLocation();
	SetActorEnableCollision(false);
	BaseRoot->SetSimulatePhysics(true);
	//UE_LOG(LogTemp, Log, TEXT("Weapon Picked Up"));
	PickupTimeline->PlayFromStart();
}

void AWeaponPickUp::OnTimelineUpdate(float Value)
{
	float currentTime = PickupTimeline->GetPlaybackPosition();

	float distanceValue = DistanceCurve->GetFloatValue(currentTime);
	float heightValue = HeightCurve->GetFloatValue(currentTime);

	FVector pickupProgressLocation
		= FMath::Lerp(PickupStartLocation, PickupTarget->GetActorLocation(), distanceValue);
	pickupProgressLocation.Z += (heightValue * PickupHeight);
	SetActorLocation(pickupProgressLocation);

	float scaleValue = ScaleCurve->GetFloatValue(currentTime);
	FVector newScale = FVector(scaleValue);
	SkeletalMesh->SetRelativeScale3D(newScale);
}

void AWeaponPickUp::OnTimelineFinished()
{
	if(PickupTarget.IsValid() && PickupTarget->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(PickupTarget.Get(), ItemCode);
	}
	Destroy();
}
