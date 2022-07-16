#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Cannon.generated.h"

class UArrowComponent;

UENUM(BlueprintType)
enum class ECannonType: uint8
{
	FireProjectile = 0 UMETA(DisplayName ="Use projectile"),
	FireTrace = 1 UMETA(DisplayName ="Use trace")
};

UCLASS()
class TANKS_API ACannon:public AActor
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UArrowComponent * SpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UParticleSystemComponent* VisualEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Effect")
	TSubclassOf<UCameraShakeBase> CameraShakeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params")
	float FireRate = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params")
	int ProjectileSeries = 3;
	
	int Series = 0;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int Ammunition = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int AmmoCapacity = 20;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params")
	ECannonType Type = ECannonType::FireProjectile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params" , meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides))
	float TraceDamage = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params", meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides))
	float TraceFireRange = 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params")
	TSubclassOf<AProjectile> ProjectileType;
	
    FTimerHandle ReloadTimerHandle;

	FTimerHandle SeriesTimer;
	
	bool ReadyToFire = false;
	
	virtual void BeginPlay() override;
	
	void Reload();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetAmmoState() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetAmmo() const;
			
	ACannon();

	void SpawnProjectile();
	
	void AIFire();
	
	void SeriesOfProjectiles();
	
	void Fire();
	
	void FireSpecial();
	
	bool IsReadyToFire() const;

	bool IsBallistic() const;

	FVector GetCurrentBallisticTarget(float FloorAbsoluteHeight) const;

	bool SetDesiredBallisticTarget(const FVector& InTarget);
	
	void SetVisibility(bool bIsVisible);

	void ReplenishmentOfAmmunition(int AddAmmo);
};