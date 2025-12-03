#include "Enemy/Base/EnemyPawn.h"

#include "GameFramework/Controller.h"
#include "Core/Common/DataTableRows.h"
#include "Combat/DamageSystem/DamagePopup.h"
#include "Combat/DamageSystem/DamagePopupSubsystem.h"
#include "Status/Base/ResourceComponent.h"
#include "Framework/System/EnemyCountSubsystem.h"
#include "Item/Pickupable/Pickup.h"
#include "Item/SpawnSystem/PickupFactorySubsystem.h"

AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	InitComponents();
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	BindOnTakeDamage();
	RegisterEnemy();
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterEnemy();

	Super::EndPlay(EndPlayReason);
}

void AEnemyPawn::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!CanDamaged(Damage)) return;

	ShowDamagePopup(Damage);
	TakeDamageProcess(Damage);
}

void AEnemyPawn::InitComponents()
{
	InitMeshComponent();
	InitResourceComponent();
	InitPopupLocation();
}

void AEnemyPawn::InitMeshComponent()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("Pawn"));
	SetRootComponent(Mesh);
}

void AEnemyPawn::InitResourceComponent()
{
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));
}

void AEnemyPawn::InitPopupLocation()
{
	PopupLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PopupLocation"));
	PopupLocation->SetupAttachment(Mesh);
	PopupLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

void AEnemyPawn::BindOnTakeDamage()
{
	OnTakeAnyDamage.AddDynamic(this, &AEnemyPawn::OnTakeDamage);
}

void AEnemyPawn::RegisterEnemy()
{
	if (UWorld* World = GetWorld())
	{
		if (UEnemyCountSubsystem* EnemyCountSubsystem = World->GetSubsystem<UEnemyCountSubsystem>())
		{
			EnemyCountSubsystem->RegisterEnemy(this);
		}
	}
}

void AEnemyPawn::UnregisterEnemy()
{
	if (UWorld* World = GetWorld())
	{
		if (UEnemyCountSubsystem* EnemyCountSubsystem = World->GetSubsystem<UEnemyCountSubsystem>())
		{
			EnemyCountSubsystem->UnregisterEnemy(this);
		}
	}
}

bool AEnemyPawn::CanDamaged(float Damage)
{
	return ResourceComponent && ResourceComponent->IsAlive()
		&& (!bIsInvincible || !FMath::IsNearlyEqual(LastDamage, Damage));	
}

void AEnemyPawn::ShowDamagePopup(float Damage)
{
	if (UDamagePopupSubsystem* DamagePopupSubsystem = GetWorld()->GetSubsystem<UDamagePopupSubsystem>())
	{
		DamagePopupSubsystem->ShowDamagePopup(PopupLocation->GetComponentLocation(), Damage);
	}
}

void AEnemyPawn::TakeDamageProcess(float Damage)
{
	ResourceComponent->TakeDamage(Damage);
	if (ResourceComponent->IsAlive())
	{
		StartInvincible(Damage);
	}
	else
	{
		OnDie();
	}
}

void AEnemyPawn::StartInvincible(float Damage)
{
	SetIsInvincible(Damage);
	StartInvincibleTimer();
}

void AEnemyPawn::SetIsInvincible(float Damage)
{
	bIsInvincible = true;
	LastDamage = Damage;
}

void AEnemyPawn::StartInvincibleTimer()
{
	FTimerDelegate InvincibleResetDelegate = FTimerDelegate::CreateWeakLambda(
		this,
		[this]() { bIsInvincible = false; }
	);

	GetWorldTimerManager().ClearTimer(InvincibleTimerHandle);
	GetWorldTimerManager().SetTimer(
		InvincibleTimerHandle,
		InvincibleResetDelegate,
		0.1f,
		false
	);
}

void AEnemyPawn::OnDie()
{
	DropItem();
	Destroy();
}

void AEnemyPawn::DropItem()
{
	if (!DropItemTable) return;

	TMap<FName, uint8*> RowMap = DropItemTable->GetRowMap();
	for (auto& Element : RowMap)
	{
		FDropItemData_TableRow* Row = reinterpret_cast<FDropItemData_TableRow*>(Element.Value);		
		SpawnPickupItem(Row->ItemCode, Row->DropItemRate);
	}
}

void AEnemyPawn::SpawnPickupItem(EItemCode ItemCode, float DropRate)
{
	if (FMath::FRand() > DropRate) return;

	if (UPickupFactorySubsystem* PickupFactorySubsystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>())
	{
		PickupFactorySubsystem->SpawnItem(
			ItemCode,
			GetActorLocation() + FVector::UpVector * 100.0f,
			GetActorRotation(),
			GetRandomDropVelocity()
		);
	}
}

FVector AEnemyPawn::GetRandomDropVelocity()
{
	FVector DropVelocity = FVector::UpVector * 500.0f;
	DropVelocity = DropVelocity.RotateAngleAxis(FMath::FRandRange(-15.0f, 15.0f), FVector::RightVector);
	DropVelocity = DropVelocity.RotateAngleAxis(FMath::FRandRange(0.0f, 360.0f), FVector::UpVector);
	return DropVelocity;
}
