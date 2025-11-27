#include "Framework/PickupFactorySubsystem_Lesson.h"

#include "Settings/PickupSettings_Lesson.h"
#include "Data/DataTableRows.h"

void UPickupFactorySubsystem_Lesson::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadPickupClassesMap();
}

APickup* UPickupFactorySubsystem_Lesson::SpawnPickup(EItemCode ItemCode, FVector Location, FRotator Rotator)
{
	APickup* pickup = nullptr;
	if(PickupClasses.Contains(ItemCode))
	{
		if (UWorld* world = GetWorld())
		{
			pickup = world->SpawnActor<APickup>(
				PickupClasses[ItemCode],
				Location,
				Rotator
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPickupFactorySubsystem_Lesson::SpawnPickup - No PickupClass found for ItemCode: %d"), static_cast<uint8>(ItemCode));
	}
	return pickup;
}

void UPickupFactorySubsystem_Lesson::LoadPickupClassesMap()
{
	const UPickupSettings_Lesson* settings = GetDefault<UPickupSettings_Lesson>();

	// IsValid 대신 IsNull 함수 사용 : UObject 포인터가 아닌 TSoftObjectPtr 또는 TSoftClassPtr 같은 스마트 포인터를 체크할 때 사용
	// IsValid 는 로딩이 완료	된 객체인지 확인하는 반면, IsNull 은 포인터가 유효한지(널이 아닌지)만 확인
	if (!settings || settings->PickupDataTable.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("UPickupFactorySubsystem_Lesson::LoadPickupClassesMap - Invalid PickupSettings or PickupDataTable is null"));
		return;
	}

	UDataTable* dataTable = settings->PickupDataTable.LoadSynchronous();
	if (!dataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("UPickupFactorySubsystem_Lesson::LoadPickupClassesMap - Failed to load PickupDataTable"));
		return;
	}

	TArray<FPickupData_TableRow*> allRows;
	dataTable->GetAllRows(TEXT("PickupDataTable"), allRows);
	for (const FPickupData_TableRow* row : allRows)
	{
		if(PickupClasses.Contains(row->ItemCode))
		{
			UE_LOG(LogTemp, Warning, TEXT("UPickupFactorySubsystem_Lesson::LoadPickupClassesMap - Duplicate ItemCode: %d"), static_cast<uint8>(row->ItemCode));
		}
		PickupClasses.Add(row->ItemCode, row->PickupClass);
	}
}
