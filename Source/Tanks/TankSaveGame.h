#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
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

USTRUCT(BlueprintType)
struct FEnemyTankData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(BlueprintReadWrite)
	FName WaypointTag;

	UPROPERTY(BlueprintReadWrite)
	float TargetRangeRadius;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	float Health;
};

USTRUCT(BlueprintType)
struct FEnemyTurretData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(BlueprintReadWrite)
	float TargetRangeRadius;

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

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

	UPROPERTY(BlueprintReadWrite)
	TArray<FEnemyTankData> EnemyTankData;

	UPROPERTY(BlueprintReadWrite)
	TArray<FEnemyTurretData> TurretData;	
};
