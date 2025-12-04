#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemporarySlotWidget.generated.h"

UCLASS()
class UNREALCPP_API UTemporarySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetItemIconImage(UTexture2D* InTexture);
	void SetCountText(int32 InCount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|UI", meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|UI", meta = (BindWidget))
	TObjectPtr<class UTextBlock> CountText = nullptr;
};
