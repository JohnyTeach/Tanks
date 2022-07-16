#include "Projectile.h"
#include "ActorPoolSubsystem.h"
#include "DamageTarget.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.005f;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	
	Mesh->SetVisibility(false);
	RootComponent = Mesh;
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MovementSpeed * DeltaTime;
	SetActorLocation(NextPosition, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > MaxDistance)
	{
		Stop();
	}
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AProjectile::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	
	StartPosition = GetActorLocation();
	
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	
}

void AProjectile::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	
	Mesh->SetVisibility(false);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
	
	if (Pool->IsActorInPool(this))
	{
		Pool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}
}

void AProjectile::Explode()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;

	//GetWorld()->DebugDrawTraceTag = "Explode Trace";

	bool bSweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		StartPos,
		EndPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		Params
	);	
	// надо что-то сделать, чтоб урон по полу не считался на игроке	
	if (bSweepResult)
	{
		for (FHitResult ExplosionHitResult : AttackHit)
		{			
			AActor* HitActor = ExplosionHitResult.GetActor();
			
			if (!HitActor)
			{				
				continue;
			}
			
			DoDamage(ExplosionHitResult);												
		}
	}
}

void AProjectile::DoDamage(const FHitResult& HitResult)
{
	AActor* OtherActor = HitResult.GetActor();
	UPrimitiveComponent* OtherComp = Cast<UPrimitiveComponent>(HitResult.GetComponent());
	
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageTarget* Damageable = Cast<IDamageTarget>(OtherActor) )
	{		
		FDamageInfo DamageData;
		DamageData.Damage = Damage;
		DamageData.Instigator = GetInstigator();
		DamageData.DamageMaker = this;
		Damageable->TakeDamage(DamageData);		
	}
	
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
		ForceVector.Normalize();
		
		OtherComp->AddImpulseAtLocation(ForceVector * MovementSpeed, HitResult.ImpactPoint);		
	}	
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == GetInstigator())
	{
		Stop();
		return;
	}

	if (bExplosive)
	{
		Explode();		
	}
	else
	{
		DoDamage(SweepResult);
	}
	
	Stop();
}
