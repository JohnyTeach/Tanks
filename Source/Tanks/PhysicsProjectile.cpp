#include "PhysicsProjectile.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("Movement Component"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
	Super::Start();

	PhysicsComponent->Velocity = GetActorForwardVector() * MovementSpeed;
	PhysicsComponent->SetComponentTickEnabled(true);
	TrailEffect->Activate(true);
	AudioComponent->Activate(true);
}

void APhysicsProjectile::Stop()
{
	PhysicsComponent->Velocity = FVector::ZeroVector;
	PhysicsComponent->SetComponentTickEnabled(false);
	TrailEffect->Deactivate();
	AudioComponent->Deactivate();

	Super::Stop();
}

void APhysicsProjectile::Tick(float DeltaSeconds)
{
	if (GetActorLocation().Z < 0)
	{
		Stop();
	}
}

void APhysicsProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == GetInstigator())
	{
		Stop();
		return;
	}
	
	if (bExplosive)
	{
		Explode();

		FTransform Transform;
		Transform.SetLocation(GetActorLocation());
		Transform.SetScale3D(FVector(3,3,3));		
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Transform);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitAudioEffect, GetActorLocation());
	}
	
	DoDamage(SweepResult);
	
	Stop();
}