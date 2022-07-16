#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "LevelPortal.h"
#include "Scorable.h"
#include "TankPawn.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKS_API ATankFactory : public AActor, public IDamageTarget, public IScorable, public IKillableActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UBoxComponent* Collision;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* LiveMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UArrowComponent* SpawnPoint;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent*  Smoke;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	class UParticleSystemComponent* HitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	class UAudioComponent* HitAudioEffect;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
	class UAudioComponent* SpawnAudioEffect;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Death")
	UStaticMeshComponent* DeadMesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	class UParticleSystemComponent*  DeathEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	class UParticleSystemComponent*  DeathSmokeEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
	class UAudioComponent* DeathAudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TSubclassOf<ATankPawn> SpawnTank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	ALevelPortal* LevelPortal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName WaypointTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	int32 SpawnRate = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	int32 NumberOfTanks = 2;

	int32 SpawnCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	int32 ScoresToDestroy = 20;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scorable")
	int32 DestructionScores = 100;

	FTimerHandle SpawnTimer;
	
	ATankFactory();

	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;

	virtual void OnHealthChanged(float Health);

	void SpawnTankTick();

	void SetHP() const;

	void OnDeath();

	void DecrementCount();
	
	virtual void BeginPlay() override;

	virtual int32 GetScores() override;
};