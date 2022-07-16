#include "AmmoBox.h"
#include "TankPawn.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AAmmoBox::AAmmoBox()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* ScnComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ScnComp;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this,&AAmmoBox::OnBeginOverlap);
}

void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();	
}

void AAmmoBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CollectedObjective(this, Other);	
	
	ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());	
	
	if (Other == PlayerPawn && IsValid(PlayerPawn))
	{
		PlayerPawn->AddHealth(AddHealth);
		
		ACannon* FirstCannon = PlayerPawn->Cannon;
		ACannon* SecondCannon = PlayerPawn->SecondaryCannon;		

		if (FirstCannon && FirstCannon->GetClass() == CannonClass)
		{
			FirstCannon->ReplenishmentOfAmmunition(Ammo);
		}
		else if (SecondCannon && SecondCannon->GetClass() == CannonClass)
		{
			SecondCannon->ReplenishmentOfAmmunition(Ammo);
		}
		else
		{
			if (FirstCannon == nullptr)
			{
				PlayerPawn->SetupCannon(CannonClass);
				
				PlayerPawn->GetCannon()->ReplenishmentOfAmmunition(Ammo);
			}
			else if (SecondCannon == nullptr)
			{
				PlayerPawn->SwapCannons();
				
				PlayerPawn->SetupCannon(CannonClass);				
			
				PlayerPawn->GetCannon()->ReplenishmentOfAmmunition(Ammo);
			
				PlayerPawn->SwapCannons();
			}
			else
			{
				PlayerPawn->SetupCannon(CannonClass);
				
				PlayerPawn->GetCannon()->ReplenishmentOfAmmunition(Ammo);
			}
		}
				
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DropAudioEffect, GetActorLocation());
		
		Destroy();
	}		
}