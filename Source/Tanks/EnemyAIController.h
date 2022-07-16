#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
#include "TargetController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class TANKS_API AEnemyAIController : public AAIController, public ITargetController
{
	GENERATED_BODY()

public:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual FVector GetTargetLocation() const override;
	
private:

	TArray<FVector> Waypoints;

	int NextWaypoint = 0;

	UPROPERTY()
	ATankPawn* TankPawn;
};