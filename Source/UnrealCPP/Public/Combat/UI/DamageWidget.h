#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "DamageWidget.generated.h"

UCLASS()
class UNREALCPP_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetDamagetText(float Damage);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PlayPopupAnimation(); // 없이 바로 직접 실행도 가능

protected:
	// meta = (BindWidget) 있어야 UMG 위젯 블루프린트에서 해당 이름의 위젯과 바인딩됩니다.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageText = nullptr;
	
	// meta = (BindWidgetAnim) 있어야 UMG 위젯 블루프린트에서 해당 이름의 애니메이션과 바인딩됩니다.
	// Transient: 직렬화에서 제외시키는 속성 (런타임에만 존재 = 런타임에 임시로 생성됨)
	UPROPERTY(Transient, meta = (BindWidgetAnim), DisplayName = "PopupAnimation")
	TObjectPtr<class UWidgetAnimation> PopupAnimation = nullptr;
};
