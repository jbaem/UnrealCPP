#include "Enemy/DamagePopupActor.h"

#include "Components/WidgetComponent.h"
#include "Widget/DamageWidget.h"

ADamagePopupActor::ADamagePopupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	SetRootComponent(DamageWidgetComponent);
	
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidgetComponent->SetDrawAtDesiredSize(true);
}

void ADamagePopupActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (DamageWidgetComponent)
	{
		DamageWidgetInstance = Cast<UDamageWidget>(DamageWidgetComponent->GetUserWidgetObject());
	}

}

void ADamagePopupActor::PopupActivate(float Damage)
{
	if (DamageWidgetInstance)
	{
		DamageWidgetInstance->SetDamagetText(Damage);
		DamageWidgetInstance->PlayPopupAnimation();
	}

	GetWorldTimerManager().SetTimer(
		LifeTimerHandle, 
		this, 
		&ADamagePopupActor::PopupDeactivate, 
		LifeTime, 
		false
	);
}

void ADamagePopupActor::PopupDeactivate()
{
	Destroy();
}
