#include "Enemy/Base/EnemyPawn.h"

#include "Components/WidgetComponent.h"
#include "Combat/UI/EnemyDamageWidget.h"

AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
	DamageWidgetComponent->SetupAttachment(RootComponent);
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	Mesh->SetCollisionProfileName(TEXT("Pawn"));
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	DeactivateDamageWidget();
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemyPawn::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyPawn::TakeDamage called with Damage: %f"), Damage);
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	SetDamage(ActualDamage);
	ActivateDamageWidget(ActualDamage);

	return ActualDamage;
}

void AEnemyPawn::SetDamage(float DamageAmount)
{
	if (DamageWidgetComponent)
	{
		if(UEnemyDamageWidget* DamageWidget = Cast<UEnemyDamageWidget>(DamageWidgetComponent->GetUserWidgetObject()))
		{
			DamageWidget->SetDamageText(DamageAmount);
		}
	}
}

void AEnemyPawn::ActivateDamageWidget(float DamageAmount)
{
	DamageWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	DamageWidgetComponent->SetHiddenInGame(false);
	DamageWidgetComponent->SetComponentTickEnabled(true);
	bIsDamageWidgetActive = true;

	if(DamageWidgetComponent)
	{
		if(UEnemyDamageWidget* DamageWidget = Cast<UEnemyDamageWidget>(DamageWidgetComponent->GetUserWidgetObject()))
		{
			DamageWidget->PlayPopAnimation();
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(DeactivateDamageWidgetTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		DeactivateDamageWidgetTimerHandle,
		this,
		&AEnemyPawn::DeactivateDamageWidget,
		0.5f,
		false
	);
}

void AEnemyPawn::DeactivateDamageWidget()
{
	if (!DamageWidgetComponent) return;

	UE_LOG(LogTemp, Warning, TEXT("AEnemyPawn::DeactivateDamageWidget called"));

	DamageWidgetComponent->SetHiddenInGame(true);
	DamageWidgetComponent->SetComponentTickEnabled(false);
	bIsDamageWidgetActive = false;

	DamageWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -10000.0f));

	GetWorld()->GetTimerManager().ClearTimer(DeactivateDamageWidgetTimerHandle);
}
