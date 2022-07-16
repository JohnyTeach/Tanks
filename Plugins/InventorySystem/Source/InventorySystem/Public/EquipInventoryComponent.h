#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "EquipInterface.h"
#include "EquipInventoryComponent.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UEquipInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	TMap<int32, EItemEquipSlot> EquipSlots;	

	UEquipInventoryComponent();

	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const override;

	virtual void UpsertItem(int32 SlotIndex, const FInventorySlotInfo& Item) override;

	virtual void RemoveItem(int32 SlotIndex) override;
};
