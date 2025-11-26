#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemCode : uint8
{
	EIC_None			UMETA(DisplayName = "None"),
	EIC_Basic			UMETA(DisplayName = "Basic"),
	EIC_IceBlade		UMETA(DisplayName = "IceBlade"),
	EIC_HellHammer		UMETA(DisplayName = "HellHammer"),
};

UENUM(BlueprintType)
enum class EWeaponCode : uint8
{
	EWC_None			UMETA(DisplayName = "None"),
	EWC_Basic			UMETA(DisplayName = "Basic"),
	EWC_IceBlade		UMETA(DisplayName = "IceBlade"),
	EWC_HellHammer		UMETA(DisplayName = "HellHammer"),
};

// Example usage: C++
// UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "ETestBitFlag"))
// uint8 testBitFlag;
// 
// Example usage: blueprint
// int 로 변수를 만들고 bitflag 체크한 후 enum 타입 설정
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETestBitFlag : uint8
{
	None = 0,
	Test1 = 1 << 0,
	Test2 = 1 << 1,
	Test3 = 1 << 2,
	Test4 = 1 << 3
};

