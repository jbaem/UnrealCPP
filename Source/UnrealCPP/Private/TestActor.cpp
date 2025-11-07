// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay(); // Play BeginPlay() of Super(Parent) Class
	
	// Category Name, Verbosity(
	// Just Color Difference in Console
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

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

