#include "Test/TestActor.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay(); // Play BeginPlay() of Super(Parent) Class
	
	UE_LOG(LogTemp, Log, TEXT("Hello World! 한글, Default Log"));
	UE_LOG(LogTemp, Warning, TEXT("Hello World!. 한글, Warning Log"));
	UE_LOG(LogTemp, Error, TEXT("Hello World!. 한글, Error Log"));
}

void ATestActor::Test_NormalFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Call Test_NormalFunction\n"));
}

void ATestActor::Test_UFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Call Test_UFunction\n"));
}

void ATestActor::Test_NativeEventFunction_Implementation()
{
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

