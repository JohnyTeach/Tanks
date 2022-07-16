#include"Cannon.h"
#include <valarray>
#include"DrawDebugHelpers.h"
#include"Projectile.h"
#include"Components/ArrowComponent.h"
#include"Components/StaticMeshComponent.h"
#include"TimerManager.h"
#include"Engine/Engine.h"
#include "ActorPoolSubsystem.h"
#include "DamageTarget.h"
#include "PhysicsProjectile.h"
#include "TankPawn.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick =false;
	
	USceneComponent * sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannonmesh"));
	Mesh->SetupAttachment(RootComponent);
	
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	SpawnPoint->SetupAttachment(Mesh);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);

	VisualEffect = CreateDefaultSubobject<UParticleSystemComponent>("VisualEffect");
	VisualEffect->SetupAttachment(RootComponent);
}

void ACannon::SpawnProjectile()
{
	UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
	FTransform SpawnTransform(SpawnPoint->GetComponentRotation(), SpawnPoint->GetComponentLocation(), FVector::OneVector);
	AProjectile* Projectile = Cast<AProjectile>(Pool->RetrieveActor(ProjectileType, SpawnTransform));
	
	if (Projectile)
	{
		Projectile->SetInstigator(GetInstigator());
		Projectile->Start();
		AudioComponent->Activate(true);
		VisualEffect->Activate(true);
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();	
	Reload();		
}

void ACannon::AIFire()
{
	if(!ReadyToFire) { return; }
	
	SpawnProjectile();
	
	ReadyToFire =false;
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle,this, &ACannon::Reload,1/FireRate,false);
}

void ACannon::Fire()
{
	if(!ReadyToFire) { return; }
	
	if (Ammunition > 0)
	{
		Ammunition--;
		switch (Type)
		{
		case ECannonType::FireProjectile:
			{
				//GEngine->AddOnScreenDebugMessage(3, 3, FColor::Blue, FString::Printf(TEXT(" Projectile")));
				
				SpawnProjectile();			
			}
			break; 
		case ECannonType::FireTrace:
			{				
				FHitResult HitResult;
				auto Start = SpawnPoint->GetComponentLocation();
				auto End = Start + SpawnPoint->GetForwardVector() * TraceFireRange;
				FCollisionObjectQueryParams ObjectQueryParams;
				ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
				ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
				ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
				ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
				ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
				ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(this);
				Params.AddIgnoredActor(GetInstigator());

				if (GetWorld()->LineTraceSingleByObjectType(HitResult,Start,End,ObjectQueryParams,Params))
				{
					DrawDebugLine(GetWorld(),Start, HitResult.Location,FColor::Red,false,2,0,5);
					//GEngine->AddOnScreenDebugMessage(3, 3, FColor::Red, FString::Printf(TEXT("Trace")));
					if (HitResult.Actor.IsValid())
					{
						auto DamageTarget = Cast<IDamageTarget>(HitResult.Actor);
						if (DamageTarget)
						{
							FDamageInfo DamageInfo;
							DamageInfo.Damage = TraceDamage;
							DamageInfo.DamageMaker = this;
							DamageInfo.Instigator = GetInstigator();
							DamageTarget->TakeDamage(DamageInfo);
						}
					}
				}
				else
				{
					DrawDebugLine(GetWorld(),Start, End,FColor::Cyan,false,2,0,5);
				}
				AudioComponent->Activate(true);
			}
			break;
		}
		
		ReadyToFire = false;		
		
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle,this, &ACannon::Reload,1/FireRate,false);

		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Green, TEXT("Ammunition: ") + FString::FromInt(Ammunition));
	}
	else
	{		
		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Red, TEXT("NO AMMO"));
	}
	
}

void ACannon::SeriesOfProjectiles()
{
	if (Ammunition<0 || ProjectileSeries == Series)
	{
		Series = 0;
		GetWorldTimerManager().ClearTimer(SeriesTimer);		
	}
	else
	{
		GetWorldTimerManager().SetTimer(SeriesTimer, this, &ACannon::SeriesOfProjectiles, 0.1,true);

		SpawnProjectile();
		
		++Series;		
	}	
}

void ACannon::FireSpecial()
{
	if (Ammunition > 0 && IsReadyToFire())
	{
		Ammunition--;
		
		//GEngine->AddOnScreenDebugMessage(3, 3, FColor::Yellow, FString::Printf(TEXT("FireSpecial")));
		
		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Green, TEXT("Ammunition: ") + FString::FromInt(Ammunition));
		
		ACannon::SeriesOfProjectiles();

		ReadyToFire = false;
		
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle,this, &ACannon::Reload,1/FireRate,false);
	}
	if(Ammunition <= 0)
	{
		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Red, TEXT("NO AMMO"));
	}	
}

bool ACannon::IsReadyToFire() const
{
	return ReadyToFire;
}

bool ACannon::IsBallistic() const
{
	return Type == ECannonType::FireProjectile && ProjectileType && ProjectileType->IsChildOf<APhysicsProjectile>();
}

FVector ACannon::GetCurrentBallisticTarget(float FloorAbsoluteHeight) const
{
	if (!IsBallistic())
	{
		return GetActorLocation();
	}

	APhysicsProjectile* DefaultProjectile = ProjectileType->GetDefaultObject<APhysicsProjectile>();
	check(DefaultProjectile);

	float Angle = FMath::DegreesToRadians(SpawnPoint->GetForwardVector().Rotation().Pitch);
	float Speed = DefaultProjectile->MovementSpeed;
	float Gravity = DefaultProjectile->PhysicsComponent->Gravity;
	if (FMath::IsNearlyZero(Gravity))
	{
		return GetActorLocation();
	}

	float Z = SpawnPoint->GetComponentLocation().Z - FloorAbsoluteHeight;

	float SqrtComp = FMath::Sqrt(FMath::Square(Speed * FMath::Sin(Angle)) + 2 * Gravity * Z);
	float Range = Speed * FMath::Cos(Angle) * (Speed * FMath::Sin(Angle) + SqrtComp) / Gravity;
	FVector FlatDirection = SpawnPoint->GetForwardVector();
	FlatDirection.Z = 0.f;
	FlatDirection.Normalize();
	FVector Result = SpawnPoint->GetComponentLocation() + FlatDirection * Range;
	Result.Z = FloorAbsoluteHeight;
	return Result;	
}

bool ACannon::SetDesiredBallisticTarget(const FVector& InTarget)
{
	if (!IsBallistic())
	{
		return false;
	}

	APhysicsProjectile* DefaultProjectile = ProjectileType->GetDefaultObject<APhysicsProjectile>();
	check(DefaultProjectile);

	float Speed = DefaultProjectile->MovementSpeed;
	float Gravity = DefaultProjectile->PhysicsComponent->Gravity;
	if (FMath::IsNearlyZero(Gravity))
	{
		return false;
	}
	float Z = SpawnPoint->GetComponentLocation().Z - InTarget.Z;
	float X = FVector::Dist2D(SpawnPoint->GetComponentLocation(), InTarget);
	float Angle = 90.f; 
	if (!FMath::IsNearlyZero(X))
	{
		float FirstSqrtComp = FMath::Pow(Speed, 4);
		float SecondSqrtComp = Gravity * (Gravity * FMath::Square(X) - 2 * (FMath::Square(Speed) * Z));
		float SqrtComp = 0.f;
		if (FirstSqrtComp > SecondSqrtComp)
		{
			SqrtComp = FMath::Sqrt(FirstSqrtComp - SecondSqrtComp);
		}
		Angle = FMath::Atan((FMath::Square(Speed) + SqrtComp) / (Gravity * X));
		Angle = FMath::RadiansToDegrees(Angle);
	}
   
	FRotator DesiredRotation = GetActorRotation();
	DesiredRotation.Pitch = Angle;
	SetActorRotation(DesiredRotation);
	return true;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

float ACannon::GetAmmoState() const
{
	return Ammunition / static_cast<float>(AmmoCapacity);
}

float ACannon::GetAmmo() const
{
	return  Ammunition;
}

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::ReplenishmentOfAmmunition(int AddAmmo)
{	
	// Надо придумать как реализовать , чтоб не добавляло больше снарядов, чем макс.доп. кол-во

	if (Ammunition + AddAmmo > AmmoCapacity)
	{
		Ammunition += std::abs(AmmoCapacity - Ammunition);
	}
	else
	{
		Ammunition += AddAmmo;
	}
	
	GEngine->AddOnScreenDebugMessage(2, 2, FColor::Green, TEXT("Ammunition: ") + FString::FromInt(Ammunition));	
}