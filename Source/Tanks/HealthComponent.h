#pragma once

#include "CoreMinimal.h"
#include "DamageTarget.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
	DECLARE_EVENT(UHealthComponent, FDeathEvent);
	
	DECLARE_EVENT_OneParam(UHealthComponent, FHealthChangedEvent, float);	
		
public:	
	
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Health")
	float MaxHealth = 10;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealthState() const;

	void TakeDamage(const FDamageInfo& DamageInfo);

	void AddHealth(float AddHealth);
		
	FDeathEvent OnDeath;

	FHealthChangedEvent OnHealthChanged;
		
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	float CurrentHealth = 0;
};