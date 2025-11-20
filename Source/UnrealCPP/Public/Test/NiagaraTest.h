#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NiagaraTest.generated.h"

UCLASS()
class UNREALCPP_API ANiagaraTest : public AActor
{
	GENERATED_BODY()
	
public:	
	ANiagaraTest();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Test")
	void TestColorChange(const FLinearColor& Color);

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UNiagaraComponent> Effect;

};
