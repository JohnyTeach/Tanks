#pragma once

#include "CoreMinimal.h"
#include "UniversalPawn.h"
#include "EnemyTankPawn.generated.h"

UCLASS()
class TANKS_API AEnemyTankPawn : public AUniversalPawn
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	virtual void OnHealthChanged(float Health) override;
	
	virtual void OnDeath() override;
};
