#include "Test/FloatingActor.h"

AFloatingActor::AFloatingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(root);
}

void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFloatingActor::OnFloatingMeshUpdate(float DeltaTime)
{
	// UpDown Movement: Constant Speed
	//ChangeMoveDirection();
	//BodyMesh->AddRelativeLocation(DeltaTime * MoveSpeed * FVector::UpVector * (bIsMoveUp ? 1 : -1));

	float MoveHeightRatio = 1 - (FMath::Cos(MoveSpeedRate * ElapsedTime) + 1) * 0.5;
	BodyMesh->SetRelativeLocation(MoveHeightRatio * MoveHeight * FVector::UpVector);

	//BodyMesh->AddRelativeRotation(FRotator(0, SpinSpeed * DeltaTime, 0));
	BodyMesh->AddRelativeRotation(DeltaTime * SpinSpeed * FRotator::MakeFromEuler(FVector::UpVector));
}

void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	OnFloatingMeshUpdate(DeltaTime);
}

void AFloatingActor::ChangeMoveDirection()
{
	float BodyMeshRelativeLocationZ = BodyMesh->GetRelativeLocation().Z;

	if (BodyMeshRelativeLocationZ > MoveHeight)
	{
		bIsMoveUp = false;
	}
	if (BodyMeshRelativeLocationZ < 0)
	{
		bIsMoveUp = true;
	}
}