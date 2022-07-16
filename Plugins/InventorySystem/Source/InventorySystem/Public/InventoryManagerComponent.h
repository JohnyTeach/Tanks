#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryMainWidget.h"
#include "InventoryWidget.h"
#include "Components/ActorComponent.h"
#include "InventoryManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UInventoryMainWidget* SelfInventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryMainWidget> MainInventoryWidgetClass;
	
	UPROPERTY()
	UInventoryComponent* SelfInventoryComponent;
	
	UPROPERTY()
	UInventoryComponent* SelfInventoryEquipComponent;	

	UPROPERTY(EditAnywhere)
	UDataTable* InventoryItemsData;

	UPROPERTY(EditAnywhere)
	int32 MinSize = 20;

	void SetBaseInventory(UInventoryComponent* InventoryComponent);

	void SetBaseEquipment(UInventoryComponent* InventoryComponent);

	void PrepareInventoryWidget(UInventoryMainWidget* Widget);

	const FInventoryItemInfo* GetItemData(const FName& Id) const;
	
	void OnItemDropEvent(UInventoryCellWidget* From, UInventoryCellWidget* To);
};