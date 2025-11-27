#include "Item/Pickup.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Player/InventoryOwner.h"

APickup::APickup()
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
	SkeletalMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	SkeletalMesh->AddRelativeRotation(FRotator(0, 0, -10.0f));

	PickupOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap"));
	PickupOverlap->SetupAttachment(BaseRoot);
	PickupOverlap->SetSphereRadius(100.0f);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision")); // 생성 직후는 바로 먹을 수 없음

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(SkeletalMesh);

	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
	
	bCanPickedUp = false;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
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

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(PickupTimerHandle);
	timerManager.SetTimer(
		PickupTimerHandle,
		[this]() { bCanPickedUp = true; },
		PickupTime,
		false
	);
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	SkeletalMesh->AddWorldRotation(FRotator(0, RotateSpeed * DeltaTime, 0));

}

void APickup::OnPickup_Implementation(AActor* Target)
{
	UE_LOG(LogTemp, Log, TEXT("Weapon PickUp OnPickup_Implementation"));
	if (!bCanPickedUp) return;
	UE_LOG(LogTemp, Log, TEXT("Weapon PickUp OnPickup_Implementation"));

	PickupTarget = Target;
	PickupStartLocation = GetActorLocation();
	SetActorEnableCollision(false);
	BaseRoot->SetSimulatePhysics(false);
	//UE_LOG(LogTemp, Log, TEXT("Weapon Picked Up"));
	PickupTimeline->PlayFromStart();
}

void APickup::OnPickupComplete_Implementation()
{
	Destroy();
}

void APickup::AddImpulse(FVector& Impulse)
{
	BaseRoot->AddImpulse(Impulse, NAME_None, true);
}

void APickup::OnTimelineUpdate(float Value)
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

void APickup::OnTimelineFinished()
{
	Execute_OnPickupComplete(this);
}
