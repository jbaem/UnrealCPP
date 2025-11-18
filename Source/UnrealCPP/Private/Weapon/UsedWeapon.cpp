#include "Weapon/UsedWeapon.h"

AUsedWeapon::AUsedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetSimulatePhysics(true);
	Mesh->SetMassOverrideInKg(NAME_None, 100.0f);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	ForceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ForceLocation"));
	ForceLocation->SetupAttachment(Mesh);
}

void AUsedWeapon::BeginPlay()
{
	Super::BeginPlay();

	FVector Direction = (GetActorForwardVector() + GetActorUpVector());
	FVector WorldLocation = GetActorLocation() + ForceLocation->GetRelativeLocation();
	//ForceLocation->GetComponentLocation();
	Mesh->AddVelocityChangeImpulseAtLocation(Direction * Force, WorldLocation, NAME_None);
}
