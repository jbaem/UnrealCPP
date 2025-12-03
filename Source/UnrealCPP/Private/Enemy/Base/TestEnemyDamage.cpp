#include "Enemy/Base/TestEnemyDamage.h"

#include "GameFramework/Controller.h"
#include "Core/Common/DataTableRows.h"
#include "Combat/DamageSystem/DamagePopupActor.h"
#include "Combat/DamageSystem/DamagePopupSubsystem.h"
#include "Status/Base/ResourceComponent.h"
#include "Framework/System/EnemyCountSubsystem.h"
#include "Item/Pickupable/Pickup.h"
#include "Item/SpawnSystem/PickupFactorySubsystem.h"

ATestEnemyDamage::ATestEnemyDamage()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("Pawn"));
	SetRootComponent(Mesh);

	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));

	PopupLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PopupLocation"));
	PopupLocation->SetupAttachment(Mesh);
	PopupLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

void ATestEnemyDamage::BeginPlay()
{
	Super::BeginPlay();
	
	OnTakeAnyDamage.AddDynamic(this, &ATestEnemyDamage::OnTakeDamage);
	
	if (UWorld* world = GetWorld())
	{
		if (UEnemyCountSubsystem* subsystem = world->GetSubsystem<UEnemyCountSubsystem>())
		{
			subsystem->RegisterEnemy(this);
		}
	}
}

void ATestEnemyDamage::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* world = GetWorld())
	{
		if (UEnemyCountSubsystem* subsystem = world->GetSubsystem<UEnemyCountSubsystem>())
		{
			subsystem->UnregisterEnemy(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void ATestEnemyDamage::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ATestEnemyDamage::OnTakeDamage Damage : %f"), Damage));
	//ADamagePopupActor* actor = GetWorld()->SpawnActor<ADamagePopupActor>(
	//	DamagePopupClass,
	//	PopupLocation->GetComponentLocation(),
	//	FRotator::ZeroRotator
	//);
	//if (actor)
	//{
	//	actor->PopupActivate(Damage);
	//}
	
	// Early returns
	if (!ResourceComponent || !ResourceComponent->IsAlive()) return;
	if (bInvincible && FMath::IsNearlyEqual(LastDamage, Damage)) return;

	UDamagePopupSubsystem* subsystem = GetWorld()->GetSubsystem<UDamagePopupSubsystem>();
	if (subsystem)
	{
		subsystem->ShowDamagePopup(Damage, PopupLocation->GetComponentLocation());
	}

	ResourceComponent->TakeDamage(Damage);

	if (ResourceComponent->IsAlive())
	{
		bInvincible = true;
		LastDamage = Damage;

		FTimerDelegate resetInvincibleDelegate = FTimerDelegate::CreateWeakLambda(
			this,
			[this]()
			{
				bInvincible = false;
			}
		);

		GetWorldTimerManager().ClearTimer(InvincibleTimerHandle);
		GetWorldTimerManager().SetTimer(
			InvincibleTimerHandle,
			resetInvincibleDelegate,
			0.1f,
			false
		);
	}
	else
	{
		OnDie();
	}
}

void ATestEnemyDamage::OnDie()
{
	DropItem();
	Destroy();
}

void ATestEnemyDamage::DropItem()
{
	if (!DropItemTable) return;
	TMap<FName, uint8*> rowMap = DropItemTable->GetRowMap();
	for (auto& element : rowMap)
	{
		FDropItemData_TableRow* row = reinterpret_cast<FDropItemData_TableRow*>(element.Value);
		if (FMath::FRand() > row->DropItemRate) continue;
		
		UPickupFactorySubsystem* subsystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();
		if (subsystem)
		{

			FVector velocity = FVector::UpVector * 500.0f;
			velocity = velocity.RotateAngleAxis(FMath::FRandRange(-15.0f, 15.0f), FVector::RightVector);
			velocity = velocity.RotateAngleAxis(FMath::FRandRange(0.0f, 360.0f), FVector::UpVector);
			subsystem->SpawnItem(
				row->ItemCode,
				GetActorLocation() + FVector::UpVector * 100.0f,
				GetActorRotation(),
				velocity
			);
		}
	}
}

void ATestEnemyDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestEnemyDamage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
