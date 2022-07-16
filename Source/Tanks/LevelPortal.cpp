#include "LevelPortal.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ALevelPortal::ALevelPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger"); 
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ALevelPortal::OnBeginOverlap);
	RootComponent = Trigger;

	DeactivatedPortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeactivatedPortal"));
	DeactivatedPortal->SetupAttachment(RootComponent);
	
	ActivePortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActivePortal"));
	ActivePortal->SetupAttachment(RootComponent);
	ActivePortal->SetVisibility(false);

	ActiveLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("ActiveLight"));
	ActiveLight->SetupAttachment(RootComponent);
	ActiveLight->SetVisibility(false);

	ActivateAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SpawnAudioEffect"));
	ActivateAudioEffect->SetupAttachment(RootComponent);
}

void ALevelPortal::ActivatePortal()
{
	if (!IsActive)
	{
		ActivateAudioEffect->Activate();
		IsActive = true; 
		DeactivatedPortal->SetVisibility(false);
		ActivePortal->SetVisibility(true);
		ActiveLight->SetVisibility(true);
	}
}

void ALevelPortal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (IsActive)
	{
		DeactivatedPortal->SetVisibility(false);
		ActivePortal->SetVisibility(true);
		ActiveLight->SetVisibility(true);
	}
	else
	{
		DeactivatedPortal->SetVisibility(true); 
		ActivePortal->SetVisibility(false);
		ActiveLight->SetVisibility(false);
	}
}

void ALevelPortal::BeginPlay()
{
	Super::BeginPlay();	
}

void ALevelPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	auto Pawn = Cast<APawn>(Other);
	
	if (Pawn && !LevelName.IsNone() && IsActive)
	{
		
		if (Pawn->GetController() == GetWorld()->GetFirstPlayerController())
		{			
			UGameplayStatics::OpenLevel(GetWorld(), LevelName);
		}
	}
}


