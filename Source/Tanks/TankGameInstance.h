#pragma once

#include "CoreMinimal.h"
#include "SavingsManager.h"
#include "Engine/GameInstance.h"
#include "TankGameInstance.generated.h"

UCLASS()
class TANKS_API UTankGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly)
    USavingsManager* SaveManager;
};
