#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "LevelPortal.generated.h"

UCLASS()
class TANKS_API ALevelPortal : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UBoxComponent* Trigger;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* DeactivatedPortal;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* ActivePortal;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	USpotLightComponent* ActiveLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* ActivateAudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	FName LevelName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	bool IsActive = false;
	
	ALevelPortal();

	void ActivatePortal();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
