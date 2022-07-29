#pragma once

#include "CoreMinimal.h"
#include "TankSaveGame.h"
#include "UObject/NoExportTypes.h"
#include "SavingsManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFromSlotAction, const FString&,SlotName);

UCLASS()
class TANKS_API USavingsManager : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite)
	UTankSaveGame* CurrentGameObject;

	UFUNCTION(BlueprintCallable)
	const TArray<FString>& GetExistingSavedSlots() const;

    UPROPERTY(BlueprintReadWrite)
	TArray<FString> ExistingSavedSlots;	

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnGameFromSlotAction OnGameLoadedFromSlot;
	
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnGameFromSlotAction OnGameSavedToSlot;	

	const FString ExistingSavedSlotsFilePath = "existing_slots.txt";
	
	void Init();
	
	void CacheExistingSavedSlotsInfo();

	void LoadExistingSavedSlotsInfo();

	UFUNCTION(BlueprintCallable)
	void DeleteSlot(FString SlotName);
	
	UFUNCTION(BlueprintCallable)
	bool DoesSaveGameExist(const FString& SlotName);
	
	UFUNCTION(BlueprintCallable)
	void LoadGame(const FString& SlotName);
	
	UFUNCTION(BlueprintCallable)
	void SaveCurrentGame(const FString& SlotName);

	void OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);
	
	void OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
