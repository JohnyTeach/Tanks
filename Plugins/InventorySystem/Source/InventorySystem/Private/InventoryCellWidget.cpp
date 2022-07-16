#include "InventoryCellWidget.h"
#include "InventoryDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CanDrag && HasItem && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Handled();
}

void UInventoryCellWidget::SetSlotVisible(const bool Value)
{
	if (ItemIconImage)
	{
		ItemIconImage->SetVisibility(Value ? ESlateVisibility::SelfHitTestInvisible: ESlateVisibility::Collapsed);
	}

	if (ItemCountTextBlock)
	{
		ItemCountTextBlock->SetVisibility(Value ? ESlateVisibility::SelfHitTestInvisible: ESlateVisibility::Collapsed);
	}
}

void UInventoryCellWidget::SetParentWidget(UInventoryWidget* InventoryWidget)
{
	ParentWidget = InventoryWidget;
}

UInventoryWidget* UInventoryCellWidget::GetParentWidget() const
{
	return ParentWidget;
}

void UInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryDragDropOperation::StaticClass());

	if (OutOperation)
	{
		if (const auto InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(OutOperation))
		{
			InventoryDragDropOperation->SourceCell = this;
			InventoryDragDropOperation->DefaultDragVisual = this;
			InventoryDragDropOperation->Pivot = EDragPivot::MouseDown;
		}
	}
}

bool UInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,	UDragDropOperation* InOperation)
{
	if (const auto InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(InOperation))
	{
		if (InventoryDragDropOperation->SourceCell != this)
		{
			if (OnItemDrop.IsBound())
			{
				OnItemDrop.Broadcast(InventoryDragDropOperation->SourceCell, this);
				return true;
			}
		}
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

const FInventorySlotInfo& UInventoryCellWidget::GetItem()
{
	return StoredItem;
}

bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo)
{
	if (HasItem) return false;

	if (SlotInfo.Count == 0)
	{
		Erase();
		return true;
	}

	if (ItemIconImage)
	{
		ItemIconImage->SetBrushFromTexture(ItemInfo.Icon.LoadSynchronous());
	}

	if (ItemCountTextBlock)
	{
		ItemCountTextBlock->SetText(FText::FromString(FString::FromInt(SlotInfo.Count)));
	}

	SetSlotVisible(true);

	HasItem = true;
	StoredItem = SlotInfo;

	return true;
}

void UInventoryCellWidget::Erase()
{
	if (!HasItem) return;

	SetSlotVisible(false);
	
	if (ItemIconImage)
	{
		ItemIconImage->SetBrush(FSlateBrush());
	}

	if (ItemCountTextBlock)
	{
		ItemCountTextBlock->SetText(FText::FromString("0"));
	}

	HasItem = false;
	StoredItem = FInventorySlotInfo();
}

bool UInventoryCellWidget::IsEmpty() const
{
	return !HasItem;
}

void UInventoryCellWidget::SetInventorySlotIndex(const int32 Index)
{
	IndexInInventory = Index;
}

int32 UInventoryCellWidget::GetIndexInInventory() const
{
	return IndexInInventory;
}