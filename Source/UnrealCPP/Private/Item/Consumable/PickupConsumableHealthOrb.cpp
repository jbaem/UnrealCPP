#include "Item/Consumable/PickupConsumableHealthOrb.h"

#include "Player/Base/ActionCharacter.h"
#include "Status/Base/ResourceComponent.h"

void APickupConsumableHealthOrb::OnConsume()
{
	Super::OnConsume();

	if(AActionCharacter* player = Cast<AActionCharacter>(PickupTarget))
	{
		if(UResourceComponent* resourceComp = player->GetResourceComponent())
		{
			//IHealthInterface* HealthInterface = Cast<IDamageInterface>(PickupTarget, Amount);
			resourceComp->RestoreHealth(25.0f);
		}
	}
}
