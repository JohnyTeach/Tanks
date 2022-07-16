#include "InventoryComponent.h"

void UInventoryComponent::Init()
{
	if (BaseInventorySet)
	{
		TArray<FInventorySlotInfo*> Slots;
		BaseInventorySet->GetAllRows<FInventorySlotInfo>("", Slots);

		for (auto i = 0; i < Slots.Num(); ++i)
		{
			UpsertItem(i, *Slots[i]);
		}
	}
}

void UInventoryComponent::SetupBaseInventorySet(UDataTable* InventorySet)
{
	BaseInventorySet = InventorySet;
	RemoveAll();
	Init();
}

FInventorySlotInfo* UInventoryComponent::GetItem(const int32 SlotIndex)
{
	return Items.Find(SlotIndex);
}

void UInventoryComponent::UpsertItem(const int32 SlotIndex, const FInventorySlotInfo& Item)
{
	RemoveItem(SlotIndex);
	Items.Add(SlotIndex, Item);
}

void UInventoryComponent::RemoveItem(const int32 SlotIndex)
{
	Items.Remove(SlotIndex);
}

int32 UInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
	const auto Info = Items.Find(SlotIndex);
	return Info && Info->Id == Item.Id ? 0 : -1;	
}

const TMap<int32, FInventorySlotInfo>& UInventoryComponent::GetItems() const
{
	return Items;
}

int32 UInventoryComponent::GetItemsNum() const
{
	return Items.Num();
}

void UInventoryComponent::RemoveAll()
{
	Items.Empty();
}