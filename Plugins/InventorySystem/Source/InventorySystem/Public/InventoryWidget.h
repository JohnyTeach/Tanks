#pragma once

#include "CoreMinimal.h"
#include "InventoryCellWidget.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryWidget.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UUniformGridPanel* ItemsGridPanel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UInventoryCellWidget* MoneyCellWidget;

	UPROPERTY(BlueprintReadOnly)
	TArray<UInventoryCellWidget*> CellWidgets;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	int32 RowSize = 5;

	UPROPERTY()
	UInventoryComponent* RepresentedInventory;

	FOnItemDrop OnItemDrop;

	virtual void NativeConstruct() override;

	void InitCellWidget(UInventoryCellWidget* Widget);

	UInventoryCellWidget* CreateCellWidget();

	void OnItemDropEvent(UInventoryCellWidget* From, UInventoryCellWidget* To);

	UFUNCTION(BlueprintCallable)
	void SetFilterByType(const TEnumAsByte<EItemType> Type);

	UFUNCTION(BlueprintCallable)
	void Init(int32 GridSizeCount);

	UFUNCTION(BlueprintCallable)
	void SetRepresentedInventory(UInventoryComponent* Component);

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetRepresentedInventory() const;
	
	UFUNCTION(BlueprintCallable)
	bool AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo, int32 SlotPosition);

	UFUNCTION(BlueprintCallable)
	bool AddCurrencySlot(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo);
};
