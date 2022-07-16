#include "TankFactory.h"

#include "HPBarWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ATankFactory::ATankFactory()
{ 
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	LiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LiveMesh"));
	LiveMesh->SetupAttachment(RootComponent);

	DeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeadMesh"));
	DeadMesh->SetupAttachment(RootComponent);
	DeadMesh->SetVisibility(false);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);

	Smoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke"));
	Smoke->SetupAttachment(RootComponent);

	SpawnAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SpawnAudioEffect"));
	SpawnAudioEffect->SetupAttachment(RootComponent);
	
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));
	HitEffect->SetupAttachment(RootComponent);
	
	HitAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioEffect"));
	HitAudioEffect->SetupAttachment(RootComponent);	

	DeathEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathEffect"));
	DeathEffect->SetupAttachment(RootComponent);
	DeathEffect->SetVisibility(false);

	DeathSmokeEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathSmokeEffect"));
	DeathSmokeEffect->SetupAttachment(RootComponent);
	DeathSmokeEffect->SetVisibility(false);

	DeathAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathAudioEffect"));
	DeathAudioEffect->SetupAttachment(RootComponent);	

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &ATankFactory::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::OnHealthChanged);

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	HPBar->SetupAttachment(RootComponent);
}

void ATankFactory::SetHP() const
{
	if (Cast<UHPBarWidget>(HPBar->GetUserWidgetObject()))
	{
		UHPBarWidget* HP = Cast<UHPBarWidget>(HPBar->GetUserWidgetObject());
		HP->SetHPValue(HealthComponent->GetHealthState(),HealthComponent->GetHealth());		
	}
}

void ATankFactory::TakeDamage(const FDamageInfo& DamageInfo)
{	
	auto PlayerScores = Cast<AUniversalPawn>(DamageInfo.Instigator);	

	HitAudioEffect->Activate(true);
	
	if (PlayerScores != nullptr && GetWorld()->GetTimerManager().IsTimerActive(SpawnTimer) && PlayerScores->Scores >= ScoresToDestroy)
	{		
		HealthComponent->TakeDamage(DamageInfo);

		HitEffect->Activate(true);

		SetHP();
	}

	if (HealthComponent && HealthComponent->GetHealth() <= 0)
	{
		KilledActor(this, DamageInfo.Instigator);
	}
	
	if(PlayerScores != nullptr && PlayerScores->Scores < ScoresToDestroy) 
	{		
		GEngine->AddOnScreenDebugMessage(3, 2, FColor::Magenta, TEXT("Need Scores to destroy: ") + FString::FromInt(ScoresToDestroy));
	}
}

void ATankFactory::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Purple, FString::Printf(TEXT("%s HP: %f "), *GetName(), Health));
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ATankFactory::SpawnTankTick,SpawnRate,true);

	SetHP();	
}

void ATankFactory::SpawnTankTick()
{		
	if (SpawnTank && SpawnCount < NumberOfTanks)
	{		
		auto Transform = SpawnPoint->GetComponentTransform();
		
		auto Tank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTank, Transform,	nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Tank->WaypointTag = WaypointTag;
		
		UGameplayStatics::FinishSpawningActor(Tank, Transform);
		
		SpawnAudioEffect->Activate(true);
		
		++SpawnCount;

		Tank->TankDeath.AddUObject(this, &ATankFactory::DecrementCount);		
	}	
}

void ATankFactory::DecrementCount()
{
	--SpawnCount;	
}

void ATankFactory::OnDeath()
{	
	DeathEffect->Activate();
	DeathAudioEffect->Activate();
	DeathSmokeEffect->Activate();

	LiveMesh->SetVisibility(false);
	Smoke->SetVisibility(false);
	DeadMesh->SetVisibility(true);
	HPBar->SetVisibility(false);
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);

	if (LevelPortal)
	{
		LevelPortal->ActivatePortal();
	}
}

int32 ATankFactory::GetScores()
{
	return DestructionScores;
}