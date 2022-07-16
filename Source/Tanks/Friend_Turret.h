#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "Cannon.h"
#include "DamageTarget.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Friend_Turret.generated.h"

UCLASS()
class TANKS_API AFriend_Turret : public APawn, public IDamageTarget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UBoxComponent* Collision;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Body;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Turret;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* HPBar;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	class UParticleSystemComponent* HitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	class UAudioComponent* HitAudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	class UParticleSystem* DeathEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	class USoundBase* DeathAudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UArrowComponent* CannonSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	USphereComponent* TargetRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TSubclassOf<AAmmoBox> DropAmmoBox;

	UPROPERTY()
	ACannon* Cannon = nullptr;
	
	AFriend_Turret();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetHP() const;

	virtual void TakeDamage(const FDamageInfo& DamageInfo);
	
	UFUNCTION()
	virtual void OnDeath();

	UFUNCTION(BlueprintCallable)
	void Fire();
	
	FHitResult TargetVisibility();	

	TWeakObjectPtr<AActor> CurrentTarget;
	
	UPROPERTY()
	TArray<AActor*> EnemyTargets;

	UFUNCTION()
	void FindBestTarget();

	UFUNCTION()
	void OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
