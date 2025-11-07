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

protected:
	int32 Data1 = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "TestVariable")
	int32 Data2 = 20.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = "TestVariable")
	int32 Data3 = 30.0f;

	UPROPERTY(VisibleInstanceOnly, Category = "TestVariable")
	int32 Data4 = 40.0f;

	UPROPERTY(EditAnywhere, Category = "TestVariable")
	int32 Data5 = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TestVariable")
	int32 Data6 = 60.0f;

	UPROPERTY(EditInstanceOnly, Category = "TestVariable")
	int32 Data7 = 70.0f;

	UPROPERTY(BlueprintReadOnly, Category = "TestVariable")
	int32 Data10 = 100.0f;

	UPROPERTY(BlueprintReadWrite, Category = "TestVariable")
	int32 Data11 = 110.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestVariable")
	int32 Data12 = 120.0f;
};
