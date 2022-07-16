#include "InventoryWidget.h"

#include "InventoryCellWidget.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (const auto& Cell : CellWidgets)
	{
		InitCellWidget(Cell);
	}
}

void UInventoryWidget::InitCellWidget(UInventoryCellWidget* Widget)
{
	if (!Widget) return;

	Widget->SetParentWidget(this);
	Widget->OnItemDrop.AddUObject(this, &UInventoryWidget::OnItemDropEvent);
}

UInventoryCellWidget* UInventoryWidget::CreateCellWidget()
{
	if (!CellWidgetClass) return nullptr;

	const auto CellWidget = CreateWidget<UInventoryCellWidget>(this, CellWidgetClass);
	CellWidgets.Add(CellWidget);

	InitCellWidget(CellWidget);

	return CellWidget;
}

void UInventoryWidget::OnItemDropEvent(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	if (!OnItemDrop.IsBound()) return;
	OnItemDrop.Broadcast(From, To);
}

void UInventoryWidget::Init(const int32 GridSizeCount)
{
	if (!ItemsGridPanel) return;
	ItemsGridPanel->ClearChildren();

	for (auto i = 0; i < GridSizeCount; ++i)
	{
		const auto CellWidget = CreateCellWidget();
		if (!CellWidget) continue;

		CellWidget->SetInventorySlotIndex(i);
		ItemsGridPanel->AddChildToUniformGrid(CellWidget, i / RowSize, i % RowSize);
	}
}

void UInventoryWidget::SetRepresentedInventory(UInventoryComponent* Component)
{
	RepresentedInventory = Component;
}

UInventoryComponent* UInventoryWidget::GetRepresentedInventory() const
{
	return RepresentedInventory;
}

bool UInventoryWidget::AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo, int32 SlotPosition)
{
	if (ItemInfo.Type == EItemType::Currency) return AddCurrencySlot(SlotInfo, ItemInfo);

	const auto FindPredicate = [SlotPosition](UInventoryCellWidget* Widget) -> bool
	{
		return Widget && Widget->GetIndexInInventory() == SlotPosition;
	};

	UInventoryCellWidget* WidgetToAddItem = nullptr;
	UInventoryCellWidget** WidgetToAddItemPtr = CellWidgets.FindByPredicate(FindPredicate);

	if (WidgetToAddItemPtr)
	{
		WidgetToAddItem = *WidgetToAddItemPtr;
	}
	else
	{
		for (const auto& CellWidget : CellWidgets)
		{
			if (!CellWidget->IsEmpty()) continue;

			WidgetToAddItem = CellWidget;
			break;
		}
	}

	if (!WidgetToAddItem) return false;
	return WidgetToAddItem->AddItem(SlotInfo, ItemInfo);
}

bool UInventoryWidget::AddCurrencySlot(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo)
{
	if (!MoneyCellWidget) return false;

	return MoneyCellWidget->AddItem(SlotInfo, ItemInfo);
}

// Simple filter
void UInventoryWidget::SetFilterByType(const TEnumAsByte<EItemType> Type)
{
	if (!ItemsGridPanel) return;

	for (const auto Widget : CellWidgets)
	{
		if (Widget->IsEmpty()) continue;

		const auto SlotType = Widget->GetItem().Type;
		Widget->SetSlotVisible(Type == SlotType || Type == EItemType::None);
	}
}
