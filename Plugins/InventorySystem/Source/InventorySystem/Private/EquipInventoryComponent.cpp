#include "EquipInventoryComponent.h"

UEquipInventoryComponent::UEquipInventoryComponent()
{
	EquipSlots.Add(0, EItemEquipSlot::Wheels);
	EquipSlots.Add(1, EItemEquipSlot::Weapon);
	EquipSlots.Add(2, EItemEquipSlot::Armor);
}

int32 UEquipInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
	return Item.Type != EItemType::Equipment
		   || !EquipSlots.Contains(SlotIndex)
		   || EquipSlots.FindChecked(SlotIndex) != Item.EquipSlot
			   ? 0
			   : 1;
}

void UEquipInventoryComponent::UpsertItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{	
	const auto Equip{ GetItem(SlotIndex) };
	const auto EquipSlot{ EquipSlots.Contains(SlotIndex) ? EquipSlots.FindChecked(SlotIndex) : EItemEquipSlot::None };
	const auto InventoryOwner = Cast<IEquipInterface>(GetOwner());
	
	if (Equip && InventoryOwner)
	{
		InventoryOwner->UnEquipItem(EquipSlot, Equip->Id);
	}
	else 
	{
		InventoryOwner->EquipItem(EquipSlot, Item.Id);
	}

	Super::UpsertItem(SlotIndex, Item);
	return;
}

void UEquipInventoryComponent::RemoveItem(int32 SlotIndex)
{
	const auto InventoryOwner = Cast<IEquipInterface>(GetOwner());
	if (!InventoryOwner)
	{
		Super::RemoveItem(SlotIndex);
		return;
	}

	const auto Slot = EquipSlots.Contains(SlotIndex) ? EquipSlots.FindChecked(SlotIndex) : EItemEquipSlot::None;

	if (const auto EquipItem = GetItem(SlotIndex))
	{
		InventoryOwner->UnEquipItem(Slot, EquipItem->Id);
	}

	Super::RemoveItem(SlotIndex);
}