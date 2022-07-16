#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	USphereComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Projectile")
	float MovementSpeed = 1000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Projectile")
	float MaxDistance = 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Damage")
	float Damage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	bool bExplosive = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (EditCondition = "bExplosive", EditConditionHides))
	float ExplodeRadius = 200;

	FTimerHandle TimerHandle;
	
	FVector StartPosition;
	
	AProjectile();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void Start();

	UFUNCTION()
	virtual void Stop();
	
	UFUNCTION()
	void Explode();

	UFUNCTION()
	void DoDamage(const FHitResult& HitResult);	

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);		
};