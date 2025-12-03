#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"

#include "InventoryDragDropOperation.generated.h"

UCLASS()
class UNREALCPP_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	int32 Index = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop")
	TWeakObjectPtr<class UItemDataAsset> ItemData = nullptr;

};
