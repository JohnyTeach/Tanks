#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "CollectibleActor.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

UCLASS()
class TANKS_API AAmmoBox : public AActor, public ICollectibleActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	class USoundBase* DropAudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Turret|Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int Ammo = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int AddHealth = 1;
	
	AAmmoBox();
	
	virtual void BeginPlay() override;	
		
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	
};
