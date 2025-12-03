#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Combat/DamageSystem/DamagePopupActor.h"

#include "DamagePopupSettings.generated.h"

// 프로젝트 세팅에서 데미지 팝업 관련 설정을 관리하는 클래스
// Config = Game 게임 카테고리에 표시 (Config/DefaultGame.ini)
// DefaultConfig 기본 게임 설정 파일에 저장됨을 의미
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Damage Popup Settings"))
class UNREALCPP_API UDamagePopupSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	// 전방 선언 안 됨
	// 소프트 클래스 포인터 : 클래스 타입을 가리키는 포인터이지만
	// 소프트 레퍼런스 : 실제로 로드되지 않은 경우에도 참조할 수 있음 (필요한 시점에 연결된 리소스를 로딩할 수 있음)
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<ADamagePopupActor> DamagePopupClass;
};
