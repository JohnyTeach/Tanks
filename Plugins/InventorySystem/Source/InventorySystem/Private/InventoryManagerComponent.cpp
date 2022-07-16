#include "InventoryManagerComponent.h"

void UInventoryManagerComponent::SetBaseInventory(UInventoryComponent* InventoryComponent)
{
	SelfInventoryComponent = InventoryComponent;	
}

void UInventoryManagerComponent::SetBaseEquipment(UInventoryComponent* InventoryComponent)
{
	SelfInventoryEquipComponent = InventoryComponent;
}

void UInventoryManagerComponent::PrepareInventoryWidget(UInventoryMainWidget* Widget)
{	
	if (!SelfInventoryComponent || !SelfInventoryEquipComponent || !InventoryItemsData || !Widget) return;
	
	SelfInventoryWidget = Widget;

	const auto LocalInventory = SelfInventoryWidget->GetInventoryWidget();
	LocalInventory->Init(FMath::Max(SelfInventoryComponent->GetItemsNum(), MinSize));
	LocalInventory->SetRepresentedInventory(SelfInventoryComponent);
	LocalInventory->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropEvent);

	const auto LocalEquipment = SelfInventoryWidget->GetEquipmentWidget();
	LocalEquipment->SetRepresentedInventory(SelfInventoryEquipComponent);
	LocalEquipment->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropEvent);

	for (auto& Item : SelfInventoryComponent->GetItems())
	{
		const auto ItemData = GetItemData(Item.Value.Id);
		if (!ItemData) continue;

		LocalInventory->AddItem(Item.Value, *ItemData, Item.Key);
	}

	for (auto& Item : SelfInventoryEquipComponent->GetItems())
	{
		const auto ItemData = GetItemData(Item.Value.Id);
		if (!ItemData) continue;

		LocalEquipment->AddItem(Item.Value, *ItemData, Item.Key);
	}
}

const FInventoryItemInfo* UInventoryManagerComponent::GetItemData(const FName& Id) const
{
	return InventoryItemsData ? InventoryItemsData->FindRow<FInventoryItemInfo>(Id, "") : nullptr;
}

void UInventoryManagerComponent::OnItemDropEvent(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	// if (!From || !To)
	// {
	// 	return;
	// }
	//
	// auto FromInventory{ From->ParentWidget->RepresentedInventory };
	// auto ToInventory{ To->ParentWidget->RepresentedInventory };
	//
	// if (!FromInventory || !ToInventory)
	// {
	// 	return;
	// }
	//
	// auto FromSlot{ From->GetItem() };
	// auto ToSlot{ To->GetItem() };
	//
	// if (FromSlot.Count < 1)
	// {
	// 	return;
	// }
	//
	// auto FromInfo{ GetItemData(FromSlot.Id) };
	// auto ToInfo{ GetItemData(ToSlot.Id) };
	//
	// if (FromInfo == nullptr || (ToSlot.Id != NAME_None && ToInfo == nullptr))
	// {
	// 	return;
	// }
	//
	// const auto MaxCount{ ToInventory->GetMaxItemAmount(To->IndexInInventory, *FromInfo) };
	// if (MaxCount == 0)
	// {
	// 	return;
	// }
	// else if (MaxCount > 0)
	// {
	// 	const auto ItemsToAdd{ FMath::Min(MaxCount, FromSlot.Count) };
	// 	ToSlot.Count = FromSlot.Count - ItemsToAdd;
	// 	ToSlot.Id = FromSlot.Id;
	// 	ToInfo = FromInfo;
	//
	// 	FromSlot.Count = ItemsToAdd;
	// }
	// else if (FromSlot.Id == ToSlot.Id)
	// {
	// 	FromSlot.Count += ToSlot.Count;
	// 	ToSlot.Count = 0;
	// }
	//
	// FromInventory->UpsertItem(From->IndexInInventory, ToSlot);
	// ToInventory->UpsertItem(To->IndexInInventory, FromSlot);
	//
	// From->Erase();
	// if (ToInfo)
	// {
	// 	From->AddItem(ToSlot, *ToInfo);
	// }
	//
	// To->Erase();
	// To->AddItem(FromSlot, *FromInfo);
	//
	// return;

	if (!From || !To) return;

	const auto FromInventoryWidget = From->GetParentWidget();
	const auto ToInventoryWidget = To->GetParentWidget();
	if (!FromInventoryWidget || !ToInventoryWidget) return;

	const auto FromInventoryComponent = FromInventoryWidget->GetRepresentedInventory();
	const auto ToInventoryComponent = ToInventoryWidget->GetRepresentedInventory();
	if (!FromInventoryComponent || !ToInventoryComponent) return;

	auto FromItem = From->GetItem();
	const auto FromItemData = GetItemData(FromItem.Id);

	auto ToItem = To->GetItem();
	const auto ToItemData = GetItemData(ToItem.Id);

	if (To->IsEmpty())
	{
		ToItem = FromItem;

		//////////////
		ToItem.Count = ToInventoryComponent->GetMaxItemAmount(To->GetIndexInInventory(), *FromItemData);

		if (ToItem.Count == 0) return;
		if (ToItem.Count == -1) ToItem.Count = FromItem.Count;

		FromItem.Count -= ToItem.Count;
		//////////////

		From->Erase();
		if (FromItem.Count)
		{
			From->AddItem(FromItem, *FromItemData);
			FromInventoryComponent->UpsertItem(From->GetIndexInInventory(), FromItem);
		}
		else
		{
			FromInventoryComponent->RemoveItem(From->GetIndexInInventory());
		}

		To->Erase();
		To->AddItem(ToItem, *FromItemData);
		ToInventoryComponent->UpsertItem(To->GetIndexInInventory(), ToItem);
	}
	else
	{
		ToItem.Count = ToInventoryComponent->GetMaxItemAmount(To->GetIndexInInventory(), *FromItemData);
		if (ToItem.Count == 0) return;

		From->Erase();
		From->AddItem(ToItem, *ToItemData);

		To->Erase();
		To->AddItem(FromItem, *FromItemData);
	}
}
