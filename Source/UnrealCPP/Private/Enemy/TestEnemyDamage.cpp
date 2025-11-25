#include "Enemy/TestEnemyDamage.h"

#include "Enemy/DamagePopupActor.h"
#include "GameFramework/Controller.h"
#include "Framework/DamagePopupSubsystem.h"
#include "Framework/EnemyCountSubsystem.h"
#include "Player/ResourceComponent.h"

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
	
	if(ResourceComponent && ResourceComponent->IsAlive())
	{
		if(!bInvincible || !FMath::IsNearlyEqual(LastDamage, Damage))
		{
			UDamagePopupSubsystem* subsystem = GetWorld()->GetSubsystem<UDamagePopupSubsystem>();
			if (subsystem)
			{
				subsystem->ShowDamagePopup(Damage, PopupLocation->GetComponentLocation());
			}

			ResourceComponent->TakeDamage(Damage);

			if(ResourceComponent->IsAlive())
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
				Destroy();
			}
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
