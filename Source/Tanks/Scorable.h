#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Scorable.generated.h"

UINTERFACE(MinimalAPI)
class UScorable : public UInterface
{
	GENERATED_BODY()
};

class TANKS_API IScorable
{
	GENERATED_BODY()

public:
	
	virtual int32 GetScores() = 0;	
};