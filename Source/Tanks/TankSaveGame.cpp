#include "TankSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UTankSaveGame::CollectData(UWorld* World)
{	
	if (APawn* SavePawn = World->GetFirstPlayerController()->GetPawn())
	{
		FMemoryWriter MemoryWriter(Bytes, true);
		
		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
		Ar.ArIsSaveGame = true;
	
		SavePawn->Serialize(Ar);		
	}	
}

void UTankSaveGame::ApplyData(UWorld* World)
{	
	if (APawn* SavePawn = World->GetFirstPlayerController()->GetPawn())
	{
		FMemoryReader MemoryReader(Bytes, true);
		
		FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
		Ar.ArIsSaveGame = true;
	
		SavePawn->Serialize(Ar);
	}	
}
