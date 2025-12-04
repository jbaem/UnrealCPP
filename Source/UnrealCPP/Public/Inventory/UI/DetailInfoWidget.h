#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

#include "DetailInfoWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class UNREALCPP_API UDetailInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void Open(class UItemDataAsset* InItemData);
	void Close();
	
	inline void SetParentPosition(const FVector2D& InPosition) { ParentPosition = InPosition; }

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemPriceText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ItemIconImage = nullptr;

private:
	void SetInfo(class UItemDataAsset* InItemData);
	void UpdateLocation();

	UCanvasPanelSlot* CanvasPanelSlot = nullptr;
	FVector2D ParentPosition;
};
