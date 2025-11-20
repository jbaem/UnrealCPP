#include "Test/NiagaraTest.h"

#include "NiagaraComponent.h"

ANiagaraTest::ANiagaraTest()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Effect->SetupAttachment(Root);
	Effect->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

void ANiagaraTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANiagaraTest::TestColorChange(const FLinearColor& Color)
{
	if (Effect)
	{
		//Effect->SetNiagaraVariableLinearColor(TEXT("EffectColor"), Color);
		//Effect->SetVariableLinearColor(TEXT("EffectColor"), Color);
		Effect->SetColorParameter(TEXT("EffectColor"), Color);
	}
}

void ANiagaraTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

