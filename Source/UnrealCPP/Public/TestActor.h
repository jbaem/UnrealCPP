// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

// "generated.h" must be positioning at last of headers
// UCLASS() : [role] Assign C++ class to UE Object system (= UObject)
//		- GC
//		- Blueprint
//		- Expose at Editor

UCLASS()
class UNREALCPP_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Test_NormalFunction();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void Test_UFunction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MyFunction")
	void Test_ImplementableFunction();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyFunction")
	void Test_NativeEventFunction();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
