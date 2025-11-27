#include "Item/PickupConsumableHealthOrb.h"

#include "Player/ActionCharacter.h"
#include "Player/ResourceComponent.h"

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
