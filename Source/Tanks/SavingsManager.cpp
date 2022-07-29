#include "SavingsManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

const TArray<FString>& USavingsManager::GetExistingSavedSlots() const
{
	return ExistingSavedSlots;
}

void USavingsManager::Init()
{
	CurrentGameObject = Cast<UTankSaveGame>(UGameplayStatics::CreateSaveGameObject(UTankSaveGame::StaticClass()));

	ExistingSavedSlots.Empty();
	
    LoadExistingSavedSlotsInfo();
}

void USavingsManager::CacheExistingSavedSlotsInfo()
{
	FString FilePath = FPaths::Combine(FPaths::ProjectSavedDir() + "SaveGames", ExistingSavedSlotsFilePath);
	
	FString ExistingSavingsArray = "";
	
	for (const FString& Slot : ExistingSavedSlots)
	{
		ExistingSavingsArray += Slot + ",";
	}
	
	FFileHelper::SaveStringToFile(ExistingSavingsArray, *FilePath, FFileHelper::EEncodingOptions::ForceUnicode, &IFileManager::Get(),FILEWRITE_EvenIfReadOnly);
}

void USavingsManager::LoadExistingSavedSlotsInfo()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	FString FilePath = FPaths::Combine(FPaths::ProjectSavedDir() + "SaveGames", ExistingSavedSlotsFilePath);
	
	if (PlatformFile.FileExists(*FilePath))
	{
		FString ExistingSavingsArray;
    	
		if (FFileHelper::LoadFileToString(ExistingSavingsArray, *FilePath))
		{
			ExistingSavingsArray.ParseIntoArray(ExistingSavedSlots, TEXT(","));
		}
	}
}

void USavingsManager::DeleteSlot(FString SlotName)
{
	LoadExistingSavedSlotsInfo();

	ExistingSavedSlots.RemoveAt(ExistingSavedSlots.Find(SlotName));
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString FilePath = FPaths::ProjectSavedDir() + "/SaveGames/" + SlotName + ".sav";

	if (PlatformFile.FileExists(*FilePath))
	{
		PlatformFile.DeleteFile(*FilePath);	
	}
	
	CacheExistingSavedSlotsInfo();
}

bool USavingsManager::DoesSaveGameExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USavingsManager::LoadGame(const FString& SlotName)
{		
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0,FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &USavingsManager::OnGameLoadedFromSlotHandle));
	
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorRelativeLocation(CurrentGameObject->TankData.Location, true);
	UKismetSystemLibrary::PrintString(CurrentGameObject,UKismetStringLibrary::Conv_VectorToString(CurrentGameObject->TankData.Location));
}

void USavingsManager::SaveCurrentGame(const FString& SlotName)
{
	CurrentGameObject->TankData.Location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	UKismetSystemLibrary::PrintString(CurrentGameObject,UKismetStringLibrary::Conv_VectorToString(CurrentGameObject->TankData.Location));	
	
	UGameplayStatics::AsyncSaveGameToSlot(CurrentGameObject, SlotName, 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &USavingsManager::OnGameSavedToSlotHandle));
}

void USavingsManager::OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentGameObject = Cast<UTankSaveGame>(SaveGame);
	
    if (OnGameLoadedFromSlot.IsBound())
    {
		OnGameLoadedFromSlot.Broadcast(SlotName);
    }	
}

void USavingsManager::OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (OnGameSavedToSlot.IsBound())
    {
		OnGameSavedToSlot.Broadcast(SlotName);
    }

	if (!ExistingSavedSlots.Contains(SlotName))
	{
		ExistingSavedSlots.Add(SlotName);
    	
		CacheExistingSavedSlotsInfo();
	}
}
