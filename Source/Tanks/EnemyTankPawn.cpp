#include "EnemyTankPawn.h"

void AEnemyTankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCannon(CannonClass);
}

void AEnemyTankPawn::OnHealthChanged(float Health)
{
	Super::OnHealthChanged(Health);
	
	GEngine->AddOnScreenDebugMessage(0, 999.f, FColor::Black, FString::Printf(TEXT("TANK HP: %f"), Health));
}

void AEnemyTankPawn::OnDeath()
{
	Super::OnDeath();

	Destroy();
}
