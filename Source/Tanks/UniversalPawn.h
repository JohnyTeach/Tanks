#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "DrawDebugHelpers.h"
#include "DamageTarget.h"
#include "EquipInterface.h"
#include "EquipInventoryComponent.h"
#include "HealthComponent.h"
#include "InventoryManagerComponent.h"
#include "Scorable.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "UniversalPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;

UCLASS()
class TANKS_API AUniversalPawn : public APawn, public IDamageTarget, public IScorable, public IEquipInterface
{
	GENERATED_BODY()
	
	DECLARE_EVENT(AUniversalPawn, TankDeathEvent);
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	UBoxComponent* Collision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Body;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Turret;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HPBar;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	UParticleSystemComponent* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	UAudioComponent* HitAudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	UParticleSystem* DeathEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	USoundBase* DeathAudioEffect;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineAudioEffect")
	UAudioComponent* EngineAudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	UArrowComponent* CannonSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	USphereComponent* TargetRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TSubclassOf<AAmmoBox> DropAmmoBox;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pawn")
	bool bIsTankPawn = false;
	
	UPROPERTY()
	ACannon* Cannon = nullptr;	

	TankDeathEvent TankDeath;	

	TWeakObjectPtr<AActor> BestTarget;
	
	UPROPERTY()
	TArray<AActor*> Targets;

	class ITargetController* TargetController;	
	
	AUniversalPawn();

	void SetHP() const;
	
	virtual void TakeDamage(const FDamageInfo& DamageInfo);

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure, Category = "Cannon")
	class ACannon* GetCannon() const;

	UFUNCTION()
	void SetupCannon(TSubclassOf<ACannon> InCannonClass);	

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void AIFire();

	UFUNCTION()
	void FireSpecial();

	UFUNCTION()
	void AddHealth(float Health);

	UFUNCTION()
	void FindBestTarget();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();
	
	UFUNCTION()
	virtual void OnDeath();
	
	UFUNCTION()
	virtual void OnHealthChanged(float Health);

	UFUNCTION(BlueprintPure, Category = "Health")
	UHealthComponent* GetHealthComponent();

	UFUNCTION()
	void OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);	

	const AActor* GetBestTarget() const
	{	
		return BestTarget.Get();
	}

	FHitResult EnemyVisibility();
	
	FRotator GetTurretRotation() const
	{
		return  Turret->GetComponentRotation();
	}
	
	int32 Scores = 0;
	
	virtual int32 GetScores() override;
};
