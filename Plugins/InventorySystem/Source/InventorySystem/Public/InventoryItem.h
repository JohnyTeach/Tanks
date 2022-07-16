#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"


UENUM()
enum class EItemType : uint8
{
	None,
	Miscellaneous,
	Currency,
	Equipment,
	Consumable
};

UENUM()
enum class EItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Mythical,
	Legendary,
	Immortal
};


UENUM()
enum class EItemEquipSlot : uint8
{
	None,
	Armor,
	Weapon,
	Wheels
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, class UInventoryCellWidget* /*From*/, class UInventoryCellWidget* /*To*/)

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FInventoryItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// General
	UPROPERTY(EditDefaultsOnly, Category="General")
	FName Id = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category="General")
	FText Name;

	UPROPERTY(EditDefaultsOnly, Category="General")
	FText Description;

	// Type
	UPROPERTY(EditDefaultsOnly, Category="Type")
	EItemType Type;

	UPROPERTY(EditDefaultsOnly, Category="Type")
	EItemRarity Rarity;

	UPROPERTY(EditDefaultsOnly, Category="Type")
	EItemEquipSlot EquipSlot;

	// View
	UPROPERTY(EditDefaultsOnly, Category="View")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category="View")
	TSoftObjectPtr<UStaticMesh> Mesh;
};

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API  FInventorySlotInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Id = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Count = INDEX_NONE;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType Type = EItemType::None;
};
