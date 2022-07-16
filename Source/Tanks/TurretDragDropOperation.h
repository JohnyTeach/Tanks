#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "TurretDragDropOperation.generated.h"

UCLASS()
class TANKS_API UTurretDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	FText TurretName;	
};
