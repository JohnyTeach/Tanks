#pragma once

#include "CoreMinimal.h"
#include "KillableActor.h"
#include "UniversalPawn.h"
#include "Turret.generated.h"

UCLASS()
class TANKS_API ATurret : public AUniversalPawn, public IKillableActor
{
	GENERATED_BODY()
	
public:
	
	ATurret();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scorable")
	int32 DestructionScores = 5;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;

	virtual int32 GetScores() override;
};
