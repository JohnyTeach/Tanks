#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetController.generated.h"

UINTERFACE(MinimalAPI)
class UTargetController : public UInterface
{
	GENERATED_BODY()
};

class TANKS_API ITargetController
{
	GENERATED_BODY()

public:

	virtual FVector GetTargetLocation() const = 0;
};