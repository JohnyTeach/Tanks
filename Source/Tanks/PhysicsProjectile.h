#pragma once

#include "CoreMinimal.h"
#include "PhysicsComponent.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

UCLASS()
class TANKS_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPhysicsComponent* PhysicsComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UParticleSystemComponent* TrailEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	USoundBase* HitAudioEffect;
	
	APhysicsProjectile();

	virtual void Start() override;
	
	virtual void Stop() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};