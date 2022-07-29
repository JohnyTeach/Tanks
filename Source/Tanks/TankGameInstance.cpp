#include "TankGameInstance.h"

void UTankGameInstance::Init()
{
	Super::Init();
	
	SaveManager = NewObject<USavingsManager>(this);
	SaveManager->Init();
}
