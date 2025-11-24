#include "Enemy/DamagePopupActor.h"

#include "Components/WidgetComponent.h"
#include "Widget/DamageWidget.h"
#include "Framework/DamagePopupSubsystem.h"

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
	if(UWorld* world = GetWorld())
	{
		if (UDamagePopupSubsystem* subsystem = world->GetSubsystem<UDamagePopupSubsystem>())
		{
			subsystem->ReturnToPool(this);
		}
		else
		{
			Destroy();	// 안전장치
		}
	}
	else
	{
		Destroy();	// 안전장치
	}
}
