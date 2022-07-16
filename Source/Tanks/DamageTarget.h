#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageTarget.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()
public:

	UPROPERTY()
	AActor* Instigator = nullptr;

	UPROPERTY()
	AActor* DamageMaker;
	
	UPROPERTY()
	float Damage;
};

UINTERFACE(MinimalAPI)
class UDamageTarget : public UInterface
{
	GENERATED_BODY()
};

class TANKS_API IDamageTarget
{
	GENERATED_BODY()

public:

	virtual void TakeDamage(const FDamageInfo& DamageInfo) = 0;
};
