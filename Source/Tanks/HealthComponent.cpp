#include "HealthComponent.h"
#include "TankFactory.h"
#include "UniversalPawn.h"

UHealthComponent::UHealthComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;
}

float UHealthComponent::GetHealth() const
{
	return  CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return  CurrentHealth / MaxHealth;
}

void UHealthComponent::TakeDamage(const FDamageInfo& DamageInfo)
{
	float PrevHealth = CurrentHealth;
	CurrentHealth -= DamageInfo.Damage;
	
	if (CurrentHealth != PrevHealth)
	{
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(CurrentHealth);
		}
	}
	
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;

		auto Instigator = Cast<AUniversalPawn>(DamageInfo.Instigator);
		auto Target = Cast<AUniversalPawn>(GetOwner());
		auto Factory = Cast<ATankFactory>(GetOwner());
		
		if (Instigator != nullptr)
		{
			if (Target != nullptr)
			{
				Instigator->Scores += Target->GetScores();
			}

			if (Factory != nullptr)
			{
				Instigator->Scores += Factory->GetScores();
			}
		}
		
		if (OnDeath.IsBound())
		{			
			OnDeath.Broadcast();					
		}
	}
}

void UHealthComponent::AddHealth(float AddHealth)
{
	CurrentHealth += AddHealth;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

