#include "Combat/DamageSystem/DamagePopupSubsystem.h"

#include "Combat/DamageSystem/DamagePopupSettings.h"

void UDamagePopupSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 코드에서 path 로드하지 말 것 (패키징 시 경로가 바뀔 수 있음)
	//DamagePopupClass = LoadClass<ADamagePopupActor>(nullptr, TEXT("/Game/Blueprints/Framework/BP_DamagePopupActor.BP_DamagePopupActor_C"));

	const UDamagePopupSettings* settings = GetDefault<UDamagePopupSettings>();
	if(settings && !settings->DamagePopupClass.IsNull())
	{
		PopupClass = settings->DamagePopupClass.LoadSynchronous();
	}
}

void UDamagePopupSubsystem::ShowDamagePopup(float Damage, FVector Location)
{
	ADamagePopupActor* selected = nullptr;
	
	if(Pool.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UDamagePopupSubsystem::ShowDamagePopup - Reusing DamagePopupActor from Pool"));
		selected = Pool.Pop();
		//selected->SetActorHiddenInGame(false);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("UDamagePopupSubsystem::ShowDamagePopup - Pool is empty, spawning new DamagePopupActor"));
		if (PopupClass && GetWorld())
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = nullptr;
			spawnParams.ObjectFlags = RF_Transient; // 풀링된 액터는 직렬화에서 제외 (레벨 저장할 때 포함 안 되게 설정)

			selected = GetWorld()->SpawnActor<ADamagePopupActor>(
				PopupClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				spawnParams
			);
#if WITH_EDITOR
			selected->SetFolderPath(TEXT("Pool")); // 월드 아웃라이너에서 Pool이라는 폴더 아래에 생성한 액터를 놓는다.
#endif
		}
	}

	if(selected)
	{
		selected->SetActorLocation(Location);
		selected->PopupActivate(Damage);
	}
}

void UDamagePopupSubsystem::ReturnToPool(ADamagePopupActor * DamagePopup)
{
	if (IsValid(DamagePopup))
	{
		//DamagePopup->SetActorHiddenInGame(true);
		Pool.Add(DamagePopup);
	}
}
