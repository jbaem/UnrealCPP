#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/CommonEnums.h"

#include "Consumable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsume, EItemCode, Consumed);

UINTERFACE(MinimalAPI)
class UConsumable : public UInterface
{
	GENERATED_BODY()
};

class UNREALCPP_API IConsumable
{
	GENERATED_BODY()

public:
	virtual FOnConsume& GetOnConsumeDelegate() = 0;
};
