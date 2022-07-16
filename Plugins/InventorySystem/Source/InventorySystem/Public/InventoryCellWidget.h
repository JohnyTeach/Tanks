#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryCellWidget.generated.h"

class UInventoryWidget;

UCLASS()
class INVENTORYSYSTEM_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UImage* ItemIconImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UTextBlock* ItemCountTextBlock;

	UPROPERTY(EditDefaultsOnly)
	bool CanDrag = true;

	UPROPERTY(EditAnywhere)
	int32 IndexInInventory = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly)
	bool HasItem;

	UPROPERTY(BlueprintReadOnly)
	FInventorySlotInfo StoredItem;

	UPROPERTY(BlueprintReadOnly)
	UInventoryWidget* ParentWidget;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	FOnItemDrop OnItemDrop;

	UFUNCTION(BlueprintCallable)
	const FInventorySlotInfo& GetItem();

	UFUNCTION(BlueprintCallable)
	bool AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo);

	UFUNCTION(BlueprintCallable)
	void Erase();

	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;

	UFUNCTION(BlueprintCallable)
	void SetInventorySlotIndex(const int32 Index);

	UFUNCTION(BlueprintPure)
	int32 GetIndexInInventory() const;

	UFUNCTION(BlueprintCallable)
	void SetSlotVisible(bool Value);

	UFUNCTION()
	void SetParentWidget(UInventoryWidget* InventoryWidget);

	UFUNCTION()
	UInventoryWidget* GetParentWidget() const;
};
