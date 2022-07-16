#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_API UPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPhysicsComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	float Gravity = 980;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	FVector Velocity = FVector::ZeroVector;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};