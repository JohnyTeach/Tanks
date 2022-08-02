#include "SavingsManager.h"
#include "TankPawn.h"
#include "Turret.h"
#include "Kismet/GameplayStatics.h"


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
	// ==================================================== SAVE PLAYER ====================================================
	auto Player = Cast<AUniversalPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(Player))
	{
		CurrentGameObject->TankData.Location = Player->GetActorLocation();
		CurrentGameObject->TankData.Rotation = Player->GetActorRotation();
		CurrentGameObject->TankData.Health = Player->GetHealthComponent()->GetHealth();
		CurrentGameObject->TankData.Ammo = Player->GetCannon()->GetAmmo();
	}
	// ==================================================== SAVE PLAYER ====================================================

	// ================================================== SAVE ENEMY TANK ==================================================
	for (auto Target : GetAllEnemyOfClass(ATankPawn::StaticClass()))
	{
		auto SaveTankPawn = Cast<ATankPawn>(Target);
		
		if (IsValid(SaveTankPawn))
		{
			FEnemyTankData NewTempTankData;
			NewTempTankData.Location = SaveTankPawn->GetActorLocation();
			NewTempTankData.Rotation = SaveTankPawn->GetActorRotation();
			NewTempTankData.Health = SaveTankPawn->GetHealthComponent()->GetHealth();
			NewTempTankData.CannonClass = SaveTankPawn->CannonClass.Get();
			NewTempTankData.TargetRangeRadius = SaveTankPawn->TargetRange->GetScaledSphereRadius();
			NewTempTankData.WaypointTag = SaveTankPawn->WaypointTag;
			
			CurrentGameObject->EnemyTankData.Add(NewTempTankData);			
		}
	}	
	// ================================================== SAVE ENEMY TANK ==================================================
	
	// ================================================ SAVE ENEMY TURRET ==================================================		
	for (auto Target : GetAllEnemyOfClass(ATurret::StaticClass()))
	{
		auto SaveTurret = Cast<ATurret>(Target);
		
		if (IsValid(SaveTurret))
		{
			FEnemyTurretData NewTempTurretData;
			NewTempTurretData.Location = SaveTurret->GetActorLocation();
			NewTempTurretData.Health = SaveTurret->GetHealthComponent()->GetHealth();
			NewTempTurretData.CannonClass = SaveTurret->CannonClass.Get();
			NewTempTurretData.TargetRangeRadius = SaveTurret->TargetRange->GetScaledSphereRadius();
			
			CurrentGameObject->TurretData.Add(NewTempTurretData);			
		}
	}	
	// ================================================ SAVE ENEMY TURRET ==================================================
	
	UGameplayStatics::AsyncSaveGameToSlot(CurrentGameObject, SlotName, 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &USavingsManager::OnGameSavedToSlotHandle));
}

void USavingsManager::SpawnEnemy(TSubclassOf<ATurret> SpawnTurret, TSubclassOf<ATankPawn> SpawnTank)
{
	// если в мире есть турели, находим все и удаляем
	for (auto Turret : GetAllEnemyOfClass(ATurret::StaticClass()))
	{
		auto CurrentTurret = Cast<ATurret>(Turret);

		if (CurrentTurret)
		{
			CurrentTurret->Destroy();
		}
			
		if (CurrentTurret->Cannon)
		{
			CurrentTurret->Cannon->Destroy();
		}
	}
	
	// если в мире есть танки, находим все и удаляем
	for (auto TankPawn : GetAllEnemyOfClass(ATankPawn::StaticClass()))
	{
		auto CurrentTankPawn = Cast<ATankPawn>(TankPawn);

		if (CurrentTankPawn)
		{
			CurrentTankPawn->Destroy();
		}
			
		if (CurrentTankPawn->Cannon)
		{
			CurrentTankPawn->Cannon->Destroy();
		}
	}

	// спавн новых турелей из сохраненного файла
	for (int i = 0; i < CurrentGameObject->TurretData.Num(); ++i)
	{				
		FVector NewTurretLocation = CurrentGameObject->TurretData[i].Location;		
		auto NewTurret = GetWorld()->SpawnActor<ATurret>(SpawnTurret, NewTurretLocation,FRotator(0));
		NewTurret->SetupCannon(CurrentGameObject->TurretData[i].CannonClass);
		NewTurret->TargetRange->SetSphereRadius(CurrentGameObject->TurretData[i].TargetRangeRadius);
		NewTurret->GetHealthComponent()->SetCurrentHealth(CurrentGameObject->TurretData[i].Health);
		NewTurret->SetHP();		
	}

	// спавн новых танков из сохраненного файла
	for (int i = 0; i < CurrentGameObject->EnemyTankData.Num(); ++i)
	{
		FTransform Transform;
		Transform.SetLocation(CurrentGameObject->EnemyTankData[i].Location);
		
		auto NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTank, Transform,	nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewTank->SetActorRotation(CurrentGameObject->EnemyTankData[i].Rotation);
		NewTank->WaypointTag = CurrentGameObject->EnemyTankData[i].WaypointTag;
		NewTank->SetupCannon(CurrentGameObject->EnemyTankData[i].CannonClass);
		NewTank->TargetRange->SetSphereRadius(CurrentGameObject->EnemyTankData[i].TargetRangeRadius);		
		NewTank->GetHealthComponent()->SetCurrentHealth(CurrentGameObject->EnemyTankData[i].Health);
		NewTank->SetHP();
		
		UGameplayStatics::FinishSpawningActor(NewTank, Transform);
	}	
}

TArray<AActor*> USavingsManager::GetAllEnemyOfClass(TSubclassOf<AActor> EnemyClass)
{
	const FName Tag = "Enemy";
	TArray<AActor*> CountEnemy;
	
	UGameplayStatics::GetAllActorsOfClassWithTag(this, EnemyClass, Tag, CountEnemy);
	
	return CountEnemy;
}

void USavingsManager::OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentGameObject = Cast<UTankSaveGame>(SaveGame);
	
    if (OnGameLoadedFromSlot.IsBound())
    {
		OnGameLoadedFromSlot.Broadcast(SlotName);
    }
	
	// ==================================================== LOAD PLAYER ====================================================
	auto Player = Cast<AUniversalPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(Player))
	{
		Player->SetActorRelativeLocation(CurrentGameObject->TankData.Location, true);
		Player->SetActorRelativeRotation(CurrentGameObject->TankData.Rotation,true);
		Player->HealthComponent->CurrentHealth = CurrentGameObject->TankData.Health;
		Player->GetCannon()->Ammunition = CurrentGameObject->TankData.Ammo;	
	}
	// ==================================================== LOAD PLAYER ====================================================
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
