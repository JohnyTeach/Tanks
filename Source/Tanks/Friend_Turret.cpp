#include "Friend_Turret.h"

#include "DrawDebugHelpers.h"
#include "HPBarWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AFriend_Turret::AFriend_Turret()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* ScnComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ScnComp;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetupAttachment(RootComponent);
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));	
	Body->SetupAttachment(RootComponent);
	
	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret"));
	Turret->SetupAttachment(Body);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannonsetup point"));
	CannonSpawnPoint->AttachToComponent(Turret, FAttachmentTransformRules::KeepRelativeTransform);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &AFriend_Turret::OnDeath);	

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	HPBar->SetupAttachment(Body);
	
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));
	HitEffect->SetupAttachment(Body);
	
	HitAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioEffect"));
	HitAudioEffect->SetupAttachment(Body);	
	
	TargetRange = CreateDefaultSubobject<USphereComponent>("TargetRange");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &AFriend_Turret::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &AFriend_Turret::OnTargetEndOverlap); 	
}

void AFriend_Turret::BeginPlay()
{
	Super::BeginPlay();
	
	if (CannonClass)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();	
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Transform);
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);	
	}
	
	SetHP();
}

void AFriend_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentTarget.IsValid() && TargetVisibility().Actor == CurrentTarget)
	{		
		auto TurretRotation = Turret->GetComponentRotation();
		auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(Turret->GetComponentLocation(), CurrentTarget->GetActorLocation());
		TargetRotation.Roll = TurretRotation.Roll;
		TargetRotation.Pitch = TurretRotation.Pitch;
		Turret->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, 0.1f));
	
		if (TurretRotation.Equals(TargetRotation, 2))
		{			
			if (Cannon)
			{
				if (Cannon->IsBallistic())
				{
					Cannon->SetDesiredBallisticTarget(CurrentTarget->GetActorLocation());
					//DrawDebugSphere(GetWorld(), Cannon->GetCurrentBallisticTarget(0.f), 100.f, 10, FColor::Red, false, 0.05f);
					Cannon->AIFire();
				}
				else
				{
					Cannon->AIFire();
				}				
			}
		}		
	}	
}

void AFriend_Turret::SetHP() const
{
	if (Cast<UHPBarWidget>(HPBar->GetUserWidgetObject()))
	{
		UHPBarWidget* HP = Cast<UHPBarWidget>(HPBar->GetUserWidgetObject());
		HP->SetHPValue(HealthComponent->GetHealthState(),HealthComponent->GetHealth());
	}
}

void AFriend_Turret::TakeDamage(const FDamageInfo& DamageInfo)
{
	//GEngine->AddOnScreenDebugMessage(5, 3, FColor::Red, FString::Printf(TEXT("!!!DAMAGE!!!")));
	HitEffect->Activate(true);
	HitAudioEffect->Activate(true);

	if (DamageInfo.Instigator != GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		HealthComponent->TakeDamage(DamageInfo);
	}
	
	SetHP();
}

FHitResult AFriend_Turret::TargetVisibility()
{
	FHitResult Vidimost;
	
	FVector Start = GetActorLocation() + FVector(0,0,20);;
	FVector End = CurrentTarget->GetActorLocation() + FVector(0,0,20);;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetInstigator());
	
	GetWorld()->LineTraceSingleByObjectType(Vidimost,Start,End,ObjectQueryParams,Params);	
	//DrawDebugLine(GetWorld(),Start, Vidimost.Location, FColor::Orange, false,2.5,0,10);		

	return Vidimost;
}

void AFriend_Turret::OnDeath()
{	
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
	
		
	Destroy();
}

void AFriend_Turret::Fire()
{
	if (Cannon)
	{		
		Cannon->AIFire();
	}
}

void AFriend_Turret::FindBestTarget()
{
	float MinDistance = 5000000;
	CurrentTarget = nullptr;
	for (auto Target:EnemyTargets)
	{
		auto Distance = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
		
		if (MinDistance > Distance)
		{						
			MinDistance = Distance;
			
			if (Target->ActorHasTag("Enemy"))
			{
				CurrentTarget = Target;	
			}														
		}
	}	
}

void AFriend_Turret::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this)
	{
		return;
	}
	
	EnemyTargets.Add(Other);
			
	if (!CurrentTarget.IsValid())
	{
		FindBestTarget();
	}
}

void AFriend_Turret::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Other == this)
	{
		return;
	}
	
	EnemyTargets.Remove(Other);

	if (Other == CurrentTarget)
	{
		FindBestTarget();
	}
}