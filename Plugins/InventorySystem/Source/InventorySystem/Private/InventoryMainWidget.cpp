#include "InventoryMainWidget.h"

UInventoryWidget* UInventoryMainWidget::GetInventoryWidget() const
{
	return InventoryWidget;
}

UInventoryWidget* UInventoryMainWidget::GetEquipmentWidget() const
{
	return EquipmentWidget;
}
