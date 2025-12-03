#include "Combat/DamageSystem/DamagePopupSubsystem.h"

#include "Combat/DamageSystem/DamagePopup.h"
#include "Combat/DamageSystem/DamagePopupSettings.h"

void UDamagePopupSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 코드에서 path 로드하지 말 것 (패키징 시 경로가 바뀔 수 있음)
	//DamagePopupClass = LoadClass<ADamagePopupActor>(nullptr, TEXT("/Game/Blueprints/Framework/BP_DamagePopupActor.BP_DamagePopupActor_C"));

	LoadDamagePopupClass();
}

void UDamagePopupSubsystem::ShowDamagePopup(FVector InLocation, float InDamage)
{
	ADamagePopup* DamagePopupSelected =
		Pool.Num() > 0 ?
		GetDamagePopupInPool() :
		GetNewDamagePopup();

	ActivateDamagePopup(DamagePopupSelected, InLocation, InDamage);
}

void UDamagePopupSubsystem::ReturnToPool(ADamagePopup* DamagePopup)
{
	if (!IsValid(DamagePopup)) return;
	//DamagePopup->SetActorHiddenInGame(true);
	Pool.Add(DamagePopup);
}


void UDamagePopupSubsystem::LoadDamagePopupClass()
{
	const UDamagePopupSettings* DamagePopupSettings = GetDefault<UDamagePopupSettings>();
	if (DamagePopupSettings && !DamagePopupSettings->DamagePopupClass.IsNull())
	{
		DamagePopupClass = DamagePopupSettings->DamagePopupClass.LoadSynchronous();
	}
}

ADamagePopup* UDamagePopupSubsystem::GetDamagePopupInPool()
{
	return Pool.Pop();
	//selected->SetActorHiddenInGame(false);
}

ADamagePopup* UDamagePopupSubsystem::GetNewDamagePopup()
{
	if (!DamagePopupClass || !GetWorld()) return nullptr;
	
	FActorSpawnParameters SpawnParams;
	SetSpawnParamsForPool(SpawnParams);
	
	return CreateDamagePopupByParams(SpawnParams);
}

void UDamagePopupSubsystem::SetSpawnParamsForPool(FActorSpawnParameters& OutSpawnParams)
{
	OutSpawnParams.Owner = nullptr;
	OutSpawnParams.ObjectFlags = RF_Transient; // 풀링된 액터는 직렬화에서 제외 (레벨 저장할 때 포함 안 되게 설정)
}

ADamagePopup* UDamagePopupSubsystem::CreateDamagePopupByParams(FActorSpawnParameters& spawnParams)
{
	ADamagePopup* DamagePopup = GetWorld()->SpawnActor<ADamagePopup>(
		DamagePopupClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		spawnParams
	);
#if WITH_EDITOR
	DamagePopup->SetFolderPath(TEXT("Pool")); // 월드 아웃라이너에서 Pool이라는 폴더 아래에 생성한 액터를 놓는다.
#endif
	return DamagePopup;
}

void UDamagePopupSubsystem::ActivateDamagePopup(ADamagePopup* InDamagePopup, FVector& Location, float Damage)
{
	if (!InDamagePopup) return;
	InDamagePopup->SetActorLocation(Location);
	InDamagePopup->ActivatePopup(Damage);
}
