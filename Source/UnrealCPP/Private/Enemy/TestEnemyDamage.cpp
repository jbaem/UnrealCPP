#include "Enemy/TestEnemyDamage.h"

#include "Enemy/DamagePopupActor.h"
#include "GameFramework/Controller.h"
#include "Framework/DamagePopupSubsystem.h"
#include "Framework/EnemyCountSubsystem.h"

ATestEnemyDamage::ATestEnemyDamage()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("Pawn"));
	SetRootComponent(Mesh);

	PopupLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PopupLocation"));
	PopupLocation->SetupAttachment(Mesh);
	PopupLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

void ATestEnemyDamage::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorld* world = GetWorld())
	{
		if (UEnemyCountSubsystem* subsystem = world->GetSubsystem<UEnemyCountSubsystem>())
		{
			subsystem->RegisterEnemy(this);
		}
	}
	
	OnTakeAnyDamage.AddDynamic(this, &ATestEnemyDamage::OnTakeDamage);
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

	UDamagePopupSubsystem* subsystem = GetWorld()->GetSubsystem<UDamagePopupSubsystem>();
	if (subsystem)
	{
		subsystem->ShowDamagePopup(Damage, PopupLocation->GetComponentLocation());
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
