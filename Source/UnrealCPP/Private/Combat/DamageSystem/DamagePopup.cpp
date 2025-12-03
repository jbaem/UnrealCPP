#include "Combat/DamageSystem/DamagePopup.h"

#include "Components/WidgetComponent.h"
#include "Combat/DamageSystem/DamagePopupSubsystem.h"
#include "Combat/UI/DamageWidget.h"

ADamagePopup::ADamagePopup()
{
	PrimaryActorTick.bCanEverTick = true;

	InitWidgetComponent();
}

void ADamagePopup::BeginPlay()
{
	Super::BeginPlay();

	CreateWidgetInstance();
}

void ADamagePopup::InitWidgetComponent()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	SetRootComponent(WidgetComponent);

	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
}

void ADamagePopup::CreateWidgetInstance()
{
	if (!WidgetComponent) return;
	WidgetInstance = Cast<UDamageWidget>(WidgetComponent->GetUserWidgetObject());
}

void ADamagePopup::ActivatePopup(float InDamage)
{
	ActivatePopupInstance(InDamage);

	GetWorldTimerManager().SetTimer(
		LifeTimerHandle, 
		this, 
		&ADamagePopup::DeactivatePopup, 
		LifeTime, 
		false
	);
}

void ADamagePopup::ActivatePopupInstance(float InDamage)
{
	if (!WidgetInstance) return;
	WidgetInstance->SetDamagetText(InDamage);
	WidgetInstance->PlayPopupAnimation();
}

void ADamagePopup::DeactivatePopup()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		Destroy();	// 안전장치
		return;
	}

	UDamagePopupSubsystem* DamagePopupSubsystem = World->GetSubsystem<UDamagePopupSubsystem>();
	if(!DamagePopupSubsystem)
	{
		Destroy();	// 안전장치
		return;
	}

	DamagePopupSubsystem->ReturnToPool(this);
}
