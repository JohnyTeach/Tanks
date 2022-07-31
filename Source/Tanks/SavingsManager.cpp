#include "SavingsManager.h"

#include <codecapi.h>

#include "Turret.h"
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
}

void USavingsManager::SaveCurrentGame(const FString& SlotName)
{
	// ==================================================== PLAYER ====================================================
	auto Player = Cast<AUniversalPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(Player))
	{
		CurrentGameObject->TankData.Location = Player->GetActorLocation();
		CurrentGameObject->TankData.Rotation = Player->GetActorRotation();
		CurrentGameObject->TankData.Health = Player->GetHealthComponent()->GetHealth();
		CurrentGameObject->TankData.Ammo = Player->GetCannon()->GetAmmo();
	}
	// ==================================================== PLAYER ====================================================
	
	// ================================================= ENEMY TANKS ==================================================

	const FName Tag = "Enemy";
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ATurret::StaticClass(), Tag, CurrentGameObject->TurretData.Turrets);
	
	for (auto Target : CurrentGameObject->TurretData.Turrets)
	{
		auto SaveTurret = Cast<ATurret>(Target);
	
		if (IsValid(SaveTurret))
		{
			CurrentGameObject->TurretData.Location = SaveTurret->GetActorLocation();
			CurrentGameObject->TurretData.Health = SaveTurret->GetHealthComponent()->GetHealth();
		}
	}
	
	// ================================================== ENEMY TANKS ==================================================

	//CurrentGameObject->CollectData(GetWorld());
	
	UGameplayStatics::AsyncSaveGameToSlot(CurrentGameObject, SlotName, 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &USavingsManager::OnGameSavedToSlotHandle));
}

void USavingsManager::OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentGameObject = Cast<UTankSaveGame>(SaveGame);
	
    if (OnGameLoadedFromSlot.IsBound())
    {
		OnGameLoadedFromSlot.Broadcast(SlotName);
    }

	//CurrentGameObject->ApplyData(GetWorld());
	
	// ==================================================== PLAYER ====================================================
	auto Player = Cast<AUniversalPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(Player))
	{
		Player->SetActorRelativeLocation(CurrentGameObject->TankData.Location, true);
		Player->SetActorRelativeRotation(CurrentGameObject->TankData.Rotation,true);
		Player->HealthComponent->CurrentHealth = CurrentGameObject->TankData.Health;
		Player->GetCannon()->Ammunition = CurrentGameObject->TankData.Ammo;	
	}
	// ==================================================== PLAYER ====================================================

	// ================================================= ENEMY TANKS ==================================================

	for (auto Target : CurrentGameObject->TurretData.Turrets)
	{
		auto LoadTurret = Cast<ATurret>(Target);

		if (IsValid(LoadTurret))
		{
			LoadTurret->SetActorLocation(CurrentGameObject->TurretData.Location);
			LoadTurret->GetHealthComponent()->CurrentHealth = CurrentGameObject->TurretData.Health;
		}		
	}
	
	// ================================================= ENEMY TANKS ==================================================
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
