#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "GameFramework/SaveGame.h"
#include "TankSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FTanksData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	float Ammo;

	UPROPERTY(BlueprintReadWrite)
	float Health;
};

UCLASS()
class TANKS_API UTankSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	FTanksData TankData;
};
