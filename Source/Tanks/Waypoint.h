#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

UCLASS()
class TANKS_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWaypoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	int Order; 
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
