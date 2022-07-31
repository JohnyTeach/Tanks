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

USTRUCT(BlueprintType)
struct FEnemyTankData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	int32 CountTanks;
	
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
	TArray<AActor*> Turrets;

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
	FEnemyTankData EnemyTankData;

	UPROPERTY(BlueprintReadWrite)
	FEnemyTurretData TurretData;


	UPROPERTY()
	TArray<uint8> Bytes;

	void CollectData(UWorld* World);

	void ApplyData(UWorld* World);
};
