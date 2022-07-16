#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlotInfo> Items;

	UPROPERTY(EditAnywhere)
	UDataTable* BaseInventorySet;

	void Init();
	
	void SetupBaseInventorySet(UDataTable* InventorySet);

	FInventorySlotInfo* GetItem(int32 SlotIndex);

	virtual void UpsertItem(int32 SlotIndex, const FInventorySlotInfo& Item);
	
	virtual void RemoveItem(int32 SlotIndex);

	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const;
	
	const TMap<int32, FInventorySlotInfo>& GetItems() const;
	
	int32 GetItemsNum() const;
	
	void RemoveAll();		
};
