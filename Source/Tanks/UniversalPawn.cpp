#include "UniversalPawn.h"
#include <solver/PxSolverDefs.h>
#include "AmmoBox.h"
#include "HPBarWidget.h"
#include "TanksHUD.h"
#include "TargetController.h"
#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AUniversalPawn::AUniversalPawn()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* ScnComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ScnComp;
	
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));	
	Body->SetupAttachment(RootComponent);
	
	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret"));
	Turret->SetupAttachment(Body);	
		
	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon Setup Point"));
	CannonSpawnPoint->AttachToComponent(Turret, FAttachmentTransformRules::KeepRelativeTransform);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDeath.AddUObject(this, &AUniversalPawn::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AUniversalPawn::OnHealthChanged);

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	HPBar->SetupAttachment(Body);
	
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Effect"));
	HitEffect->SetupAttachment(Body);
	
	HitAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Hit Audio Effect"));
	HitAudioEffect->SetupAttachment(Body);	
	
	TargetRange = CreateDefaultSubobject<USphereComponent>(TEXT("Target Range"));
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &AUniversalPawn::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &AUniversalPawn::OnTargetEndOverlap);	
}

void AUniversalPawn::SetHP() const
{
	if (Cast<UHPBarWidget>(HPBar->GetUserWidgetObject()))
	{
		UHPBarWidget* HP = Cast<UHPBarWidget>(HPBar->GetUserWidgetObject());
		HP->SetHPValue(HealthComponent->GetHealthState(),HealthComponent->GetHealth());
	}
}

void AUniversalPawn::TakeDamage(const FDamageInfo& DamageInfo)
{
	//GEngine->AddOnScreenDebugMessage(5, 3, FColor::Red, FString::Printf(TEXT("!!!DAMAGE!!!")));
	HitEffect->Activate(true);
	HitAudioEffect->Activate(true);
	
	HealthComponent->TakeDamage(DamageInfo);	
	
	SetHP();
}

UHealthComponent* AUniversalPawn::GetHealthComponent()
{
	return HealthComponent;
}

void AUniversalPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TargetController = Cast<ITargetController>(NewController);
}

void AUniversalPawn::BeginPlay()
{
	Super::BeginPlay();	
	
	SetHP();	
}

void AUniversalPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

ACannon* AUniversalPawn::GetCannon() const
{
	return Cannon;	
}

void AUniversalPawn::SetupCannon(TSubclassOf<ACannon> InCannonClass)
{
	if(Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}
	
	CannonClass = InCannonClass;
	
	if (CannonClass)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();
		
		FActorSpawnParameters Parameters;
		
		Parameters.Instigator = this;
		
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Transform, Parameters);
		
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void AUniversalPawn::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Red, TEXT("!!!NO GUN TO SHOT!!!"));
	}
}

void AUniversalPawn::AIFire()
{
	if (Cannon)
	{	
		Cannon->AIFire();
	}
}

void AUniversalPawn::FireSpecial()
{
	if(Cannon)
	{
		Cannon->FireSpecial();
	}
}

void AUniversalPawn::AddHealth(float Health)
{
	if (HealthComponent)
	{
		HealthComponent->AddHealth(Health);
	}
}

void AUniversalPawn::FindBestTarget()
{
	float MinDistance = 5000000;
	
	BestTarget = nullptr;
	
	for (auto Target:Targets)
	{
		auto Distance = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
		
		if (MinDistance > Distance)
		{				
			if (GetWorld()->GetFirstPlayerController() == Target->GetInstigatorController() || Target->ActorHasTag("Friend"))
			{				
				MinDistance = Distance;
				
				BestTarget = Target;				
			}			
		}
	}	
}

FHitResult AUniversalPawn::EnemyVisibility()
{
	FHitResult Visibility;
	
	FVector Start = GetActorLocation() + FVector(0,0,20);
	
	FVector End = BestTarget->GetActorLocation() + FVector(0,0,20);
		
	FCollisionObjectQueryParams ObjectQueryParams;
	
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
	
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
	
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		
	FCollisionQueryParams Params;
	
	Params.AddIgnoredActor(this);
	
	Params.AddIgnoredActor(GetInstigator());
		
	GetWorld()->LineTraceSingleByObjectType(Visibility,Start,End,ObjectQueryParams,Params);	
	//DrawDebugLine(GetWorld(), Start, Visibility.Location, FColor::Orange, false, 1, 0, 10);

	return Visibility;
}

void AUniversalPawn::OnDeath()
{	
	if (IsPlayerControlled())
	{
		ATanksHUD* HUD = Cast<ATanksHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		
		HUD->UseWidget(EWidgetID::GameOver);		
	}
	
	if (TankDeath.IsBound())
	{			
		TankDeath.Broadcast();					
	}

	FTransform Transform;	
	Transform.SetLocation(GetActorLocation());
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, Transform,true);	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathAudioEffect, GetActorLocation());
	
	if (Cannon)
	{
		Cannon->Destroy();
	}
	
	auto Ammobox = GetWorld()->SpawnActorDeferred<AAmmoBox>(DropAmmoBox, Transform,	nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);	
	UGameplayStatics::FinishSpawningActor(Ammobox, Transform);

	if (EngineAudioEffect)
	{
		EngineAudioEffect->Stop();
	}
		
	Destroy();
}

void AUniversalPawn::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Purple, FString::Printf(TEXT("%s HP: %f "), *GetName(), Health));
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Black, TEXT("HP: ") + FString::FromInt(Health));	
}

void AUniversalPawn::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this)
	{
		return;
	}
	
	Targets.Add(Other);
			
	if (!BestTarget.IsValid())
	{
		FindBestTarget();
	}
}

void AUniversalPawn::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Other == this)
	{
		return;
	}
	
	Targets.Remove(Other);

	if (Other == BestTarget)
	{
		FindBestTarget();
	}
}

int32 AUniversalPawn::GetScores()
{
	return Scores;
}