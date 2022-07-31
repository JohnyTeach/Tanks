#include "Turret.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATurret::ATurret()
{
	
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	if (CannonClass)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();	
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Transform);
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);	
	}	
}

void ATurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (BestTarget.IsValid() && EnemyVisibility().Actor == BestTarget)
	{			
		auto TurretRotation = Turret->GetComponentRotation();
		auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(Turret->GetComponentLocation(), BestTarget->GetActorLocation());
		TargetRotation.Roll = TurretRotation.Roll;
		TargetRotation.Pitch = TurretRotation.Pitch;
		Turret->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, 0.1f));

		if (TurretRotation.Equals(TargetRotation, 2))
		{			
			if (Cannon)
			{
				if (Cannon->IsBallistic())
				{
					Cannon->SetDesiredBallisticTarget(BestTarget->GetActorLocation());
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

void ATurret::TakeDamage(const FDamageInfo& DamageInfo)
{
	Super::TakeDamage(DamageInfo);

	if (HealthComponent && HealthComponent->GetHealth() <= 0)
	{
		KilledActor(this, DamageInfo.Instigator);
	}
}

int32 ATurret::GetScores()
{
	return DestructionScores;
}

