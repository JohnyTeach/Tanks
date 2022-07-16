#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorPoolSubsystem.generated.h"

UCLASS()
class TANKS_API UActorPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	AActor* RetrieveActor(UClass* Class, const FTransform& Transform);
	
	void ReturnActor(AActor* Actor);
	
	bool IsActorInPool(AActor* Actor) const;	

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

	UFUNCTION()
	void OnActorDestroyed(AActor* Actor);

	UPROPERTY()
	TArray<class AActor*> Actors;

	TMap<UClass*, TArray<class AActor*>> InactiveActors;
};
